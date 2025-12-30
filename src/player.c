#include "player.h"
#include "config.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

Player PlayerInit(void) {
  Player player = {
      .sensitivity = {0.001f, 0.001f},
      .body = {0},
      .lookRotation = {0},
      .isRunning = 0,
      .headTimer = 0.0f,
      .walkLerp = 0.0f,
      .headLerp = 0.0f,
      .lean = {0},
  };

  return player;
}

static void UpdateBody(Body *body, const float rot, const char side,
                       const char forward, const bool jumpPressed,
                       const bool crouchHold, const bool running) {
  Vector2 input = (Vector2){(float)side, (float)-forward};

#if defined(NORMALIZE_INPUT)
  if ((side != 0) && (forward != 0))
    input = Vector2Normalize(input);
#endif

  float delta = GetFrameTime();

  if (!body->isGrounded)
    body->velocity.y -= GRAVITY * delta;

  if (body->isGrounded && jumpPressed) {
    body->velocity.y = JUMP_FORCE;
    body->isGrounded = false;
  }

  Vector3 front = (Vector3){sinf(rot), 0.0f, cosf(rot)};
  Vector3 right = (Vector3){cosf(-rot), 0.0f, sinf(-rot)};

  Vector3 desiredDir = (Vector3){
      input.x * right.x + input.y * front.x,
      0.0f,
      input.x * right.z + input.y * front.z,
  };
  body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL * delta);

  float decel = (body->isGrounded ? FRICTION : AIR_DRAG);
  Vector3 hvel =
      (Vector3){body->velocity.x * decel, 0.0f, body->velocity.z * decel};

  float hvelLength = Vector3Length(hvel);
  if (hvelLength < (BASE_SPEED * 0.01f))
    hvel = (Vector3){0};

  float speed = Vector3DotProduct(hvel, body->dir);

  float maxSpeed = (crouchHold ? CROUCH_SPEED : BASE_SPEED);
  maxSpeed = (running ? RUN_SPEED : maxSpeed);

  float accel = Clamp(maxSpeed - speed, 0.0f, MAX_ACCEL * delta);
  hvel.x += body->dir.x * accel;
  hvel.z += body->dir.z * accel;

  body->velocity.x = hvel.x;
  body->velocity.z = hvel.z;

  body->position.x += body->velocity.x * delta;
  body->position.y += body->velocity.y * delta;
  body->position.z += body->velocity.z * delta;

  if (body->position.y <= 0.0f) {
    body->position.y = 0.0f;
    body->velocity.y = 0.0f;
    body->isGrounded = true;
  }
}

void PlayerUpdate(Player *player, const float delta) {
  Vector2 mouseDelta = GetMouseDelta();
  player->lookRotation.x -= mouseDelta.x * player->sensitivity.x;
  player->lookRotation.y += mouseDelta.y * player->sensitivity.y;

  char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
  char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));

  bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
  bool running = IsKeyDown(KEY_LEFT_SHIFT);

  UpdateBody(&player->body, player->lookRotation.x, sideway, forward,
             IsKeyPressed(KEY_SPACE), crouching, running);

  player->headLerp =
      Lerp(player->headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT),
           20.0f * delta);

  player->camera.position = (Vector3){
      player->body.position.x,
      player->body.position.y + (BOTTOM_HEIGHT + player->headLerp),
      player->body.position.z,
  };

  float headBobSpeed = 3.0f;
  float targetFov = 60.0f;

  if (running) {
    headBobSpeed = 5.0f;
    targetFov = 50.0f;
  } else if (crouching) {
    headBobSpeed = 1.5f;
    targetFov = 60.0f;
  }

  if (player->body.isGrounded && (forward || sideway)) {
    player->headTimer += delta * headBobSpeed;
    player->walkLerp = Lerp(player->walkLerp, 1.0f, 10.0f * delta);
    player->camera.fovy = Lerp(player->camera.fovy, targetFov, 5.0f * delta);
  } else {
    player->walkLerp = Lerp(player->walkLerp, 0.0f, 10.0f * delta);
    player->camera.fovy = Lerp(player->camera.fovy, 60.0f, 5.0f * delta);
  }

  player->lean.x = Lerp(player->lean.x, sideway * 0.02f, 10.0f * delta);
  player->lean.y = Lerp(player->lean.y, forward * 0.015f, 10.0f * delta);
}

void UpdateCameraFPS(Player *player) {
  const Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};
  const Vector3 targetOffset = (Vector3){0.0f, 0.0f, -1.0f};

  Vector3 yaw =
      Vector3RotateByAxisAngle(targetOffset, up, player->lookRotation.x);

  float maxAngelUp = Vector3Angle(up, yaw);
  maxAngelUp -= 0.001f;
  if (-(player->lookRotation.y) > maxAngelUp) {
    player->lookRotation.y = -maxAngelUp;
  }

  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f;
  maxAngleDown += 0.001f;
  if (-(player->lookRotation.y) < maxAngleDown) {
    player->lookRotation.y = -maxAngleDown;
  }

  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  float pitchAngle = -player->lookRotation.y - player->lean.y;
  pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f);
  Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

  float headSin = sinf(player->headTimer * PI);
  float headCos = cosf(player->headTimer * PI);
  const float stepRotation = 0.01f;
  player->camera.up = Vector3RotateByAxisAngle(
      up, pitch, headSin * stepRotation + player->lean.x);

  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
  bobbing.y = fabsf(headCos * bobUp);

  player->camera.position = Vector3Add(player->camera.position,
                                       Vector3Scale(bobbing, player->walkLerp));
  player->camera.target = Vector3Add(player->camera.position, pitch);
}

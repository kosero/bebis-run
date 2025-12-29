#include "player.h"
#include "config.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

Vector2 sensitivity = {0.001f, 0.001f};
Body player = {0};
Vector2 lookRotation = {0};
float headTimer = 0.0f;
float walkLerp = 0.0f;
float headLerp = STAND_HEIGHT;
Vector2 lean = {0};

void PlayerUpdate(Camera *camera, float delta) {
  Vector2 mouseDelta = GetMouseDelta();
  lookRotation.x -= mouseDelta.x * sensitivity.x;
  lookRotation.y += mouseDelta.y * sensitivity.y;

  char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
  char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
  bool crouching = IsKeyDown(KEY_LEFT_CONTROL);

  UpdateBody(&player, lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE),
             crouching);

  headLerp =
      Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f * delta);

  camera->position = (Vector3){
      player.position.x,
      player.position.y + (BOTTOM_HEIGHT + headLerp),
      player.position.z,
  };

  if (player.isGrounded && (forward || sideway)) {
    headTimer += delta * 3.0f;
    walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
    camera->fovy = Lerp(camera->fovy, 55.0f, 5.0f * delta);
  } else {
    walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
    camera->fovy = Lerp(camera->fovy, 60.0f, 5.0f * delta);
  }

  lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
  lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);
}

void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold) {
  Vector2 input = (Vector2){(float)side, (float)-forward};

#if defined(NORMALIZE_INPUT)
  if ((side != 0) && (forward != 0))
    = input = Vector2Normalize(input);
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
  if (hvelLength < (MAX_SPEED * 0.01f))
    hvel = (Vector3){0};

  float speed = Vector3DotProduct(hvel, body->dir);

  float maxSpeed = (crouchHold ? CROUCH_SPEED : MAX_SPEED);
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

void UpdateCameraFPS(Camera *camera) {
  const Vector3 up = (Vector3){0.0f, 1.0f, 0.0f};
  const Vector3 targetOffset = (Vector3){0.0f, 0.0f, -1.0f};

  Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

  float maxAngelUp = Vector3Angle(up, yaw);
  maxAngelUp -= 0.001f;
  if (-(lookRotation.y) > maxAngelUp) {
    lookRotation.y = -maxAngelUp;
  }

  float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
  maxAngleDown *= -1.0f;
  maxAngleDown += 0.001f;
  if (-(lookRotation.y) < maxAngleDown) {
    lookRotation.y = -maxAngleDown;
  }

  Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

  float pitchAngle = -lookRotation.y - lean.y;
  pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f);
  Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

  float headSin = sinf(headTimer * PI);
  float headCos = cosf(headTimer * PI);
  const float stepRotation = 0.01f;
  camera->up =
      Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);

  const float bobSide = 0.1f;
  const float bobUp = 0.15f;
  Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
  bobbing.y = fabsf(headCos * bobUp);

  camera->position =
      Vector3Add(camera->position, Vector3Scale(bobbing, walkLerp));
  camera->target = Vector3Add(camera->position, pitch);
}

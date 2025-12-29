#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
const float MAX_SPEED = 20.0f;
const float CROUCH_SPEED = 5.0f;
const float JUMP_FORCE = 12.0f;
const float MAX_ACCEL = 150.0f;
const float FRICTION = 0.86f;
const float AIR_DRAG = 0.98f;
const float CONTROL = 15.0f;
const float CROUCH_HEIGHT = 0.0;
const float STAND_HEIGHT = 1.0f;
const float BOTTOM_HEIGHT = 0.5f;

typedef struct {
  Vector3 position;
  Vector3 velocity;
  Vector3 dir;
  bool isGrounded;
} Body;

static Vector2 sensivitiy = {0.001f, 0.001f};

static Body player = {0};
static Vector2 lookRotation = {0};
static float headTimer = 0.0f;
static float walkLerp = 0.0f;
static float headLerp = STAND_HEIGHT;
static Vector2 lean = {0};

void UpdateCameraFps(Camera *camera);
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold);

#endif // !PLAYER_H

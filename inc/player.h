#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define MAX_SPEED 20.0f
#define CROUCH_SPEED 5.0f
#define JUMP_FORCE 12.0f
#define MAX_ACCEL 150.0f
#define FRICTION 0.86f
#define AIR_DRAG 0.98f
#define CONTROL 15.0f
#define CROUCH_HEIGHT 0.0
#define STAND_HEIGHT 1.0f
#define BOTTOM_HEIGHT 0.5f

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

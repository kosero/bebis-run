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

extern Vector2 sensitivity;
extern Body player;
extern Vector2 lookRotation;
extern float headTimer;
extern float walkLerp;
extern float headLerp;
extern Vector2 lean;

void UpdateCameraFPS(Camera *camera);
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold);

#endif // !PLAYER_H

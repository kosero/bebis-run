
#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

#define BASE_SPEED 8.0f
#define RUN_SPEED 20.0f
#define CROUCH_SPEED 3.0f
#define JUMP_FORCE 12.0f
#define MAX_ACCEL 1000.0f
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

typedef struct Player {
  Camera camera;
  Vector2 sensitivity;
  Body body;
  Vector2 lookRotation;
  bool isRunning;
  float headTimer;
  float walkLerp;
  float headLerp;
  Vector2 lean;
} Player;

Player PlayerInit(void);
void PlayerUpdate(Player *player, float delta);
void UpdateCameraFPS(Player *player);
void UpdateBody(Body *body, float rot, char side, char forward,
                bool jumpPressed, bool crouchHold, bool running);

#endif // !PLAYER_H

#include "config.h"
#include "player.h"

#include <raylib.h>
#include <raymath.h>

int main(void) {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

  Camera camera = {0};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = (Vector3){
      player.position.x,
      player.position.y + (BOTTOM_HEIGHT + headLerp),
      player.position.z,
  };

  UpdateCameraFPS(&camera);
  DisableCursor();
  ToggleFullscreen();

  Texture2D bebis = LoadTexture("./resource/bebis.png");
  Vector3 bebisPos = {5.0f, 1.5f, 5.0f};
  Vector3 bebisVel = {0};

  float bebisBaseSpeed = 10.0f;
  float bebisRunSpeed = 20.0f;

  bool gameOver = false;

  while (!WindowShouldClose()) {
    float delta = GetFrameTime();

    // Player
    if (!gameOver) {
      Vector2 mouseDelta = GetMouseDelta();
      lookRotation.x -= mouseDelta.x * sensitivity.x;
      lookRotation.y += mouseDelta.y * sensitivity.y;

      char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
      char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
      bool crouching = IsKeyDown(KEY_LEFT_CONTROL);

      UpdateBody(&player, lookRotation.x, sideway, forward,
                 IsKeyPressed(KEY_SPACE), crouching);

      headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT),
                      20.0f * delta);

      camera.position = (Vector3){
          player.position.x,
          player.position.y + (BOTTOM_HEIGHT + headLerp),
          player.position.z,
      };

      if (player.isGrounded && (forward || sideway)) {
        headTimer += delta * 3.0f;
        walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
        camera.fovy = Lerp(camera.fovy, 55.0f, 5.0f * delta);
      } else {
        walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
        camera.fovy = Lerp(camera.fovy, 60.0f, 5.0f * delta);
      }

      lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
      lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);
    }

    // bebis
    Vector3 dir = Vector3Subtract(player.position, bebisPos);
    dir.y = 0.0f;

    float distance = Vector3Length(dir);

    if (distance > 0.01f) {
      dir = Vector3Normalize(dir);

      float speed = (distance < 5.0f) ? bebisRunSpeed : bebisBaseSpeed;

      Vector3 desiredVel = Vector3Scale(dir, speed);
      bebisVel = Vector3Lerp(bebisVel, desiredVel, 5.0f * delta);
      bebisPos = Vector3Add(bebisPos, Vector3Scale(bebisVel, delta));
    }

    if (distance < 0.8f) {
      gameOver = true;
    }

    UpdateCameraFPS(&camera);

    BeginDrawing();
    ClearBackground(BLACK);

    if (!gameOver) {
      DrawFPS(15, 15);
    }

    BeginMode3D(camera);

    DrawBillboard(camera, bebis, bebisPos, 2.0f, WHITE);
    DrawGrid(250, 1.0f);

    EndMode3D();

    if (gameOver) {
      DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(),
                    Fade(BLACK, 0.6f));
      DrawText("GAME OVER", GetScreenWidth() / 2 - 150,
               GetScreenHeight() / 2 - 40, 60, RED);
      player.position = (Vector3){0, 0, 0};
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

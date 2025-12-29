#include "bebis.h"
#include "config.h"
#include "game-manager.h"
#include "player.h"

#include <raylib.h>
#include <raymath.h>

GameState currentGameState = MainMenu;

int main(void) {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
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

  Vector3 pos = {0};
  Bebis bebis = BebisInit(pos, 10.0f, 15.0f, "./resource/bebis.png");

  while (!WindowShouldClose()) {
    float delta = GetFrameTime();

    // player
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

    // bebis
    BebisUpdate(&bebis, &player, delta);

    UpdateCameraFPS(&camera);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(15, 15);

    BeginMode3D(camera);

    DrawBillboard(camera, bebis.texture, bebis.position, 2.0f, WHITE);
    DrawGrid(250, 1.0f);

    EndMode3D();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

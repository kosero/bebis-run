#include "bebis.h"
#include "config.h"
#include "game-manager.h"
#include "player.h"
#include "raygui.h"
#include <raylib.h>
#include <raymath.h>

GameState currentGameState = GameLoop;

int main(void) {
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

  Player player = PlayerInit();
  Camera camera = {0};
  camera.fovy = 60.0f;
  camera.projection = CAMERA_PERSPECTIVE;
  camera.position = (Vector3){
      player.body.position.x,
      player.body.position.y + (BOTTOM_HEIGHT + player.headLerp),
      player.body.position.z,
  };
  player.camera = camera;

  UpdateCameraFPS(&player);
  DisableCursor();
  ToggleFullscreen();

  Vector3 pos = {0};
  Bebis bebis = BebisInit(pos, 10.0f, 15.0f, "./resource/bebis.png");

  while (!WindowShouldClose()) {
    float delta = GetFrameTime();

    switch (currentGameState) {
    case MainMenu:
      break;
    case GameLoop:
      PlayerUpdate(&player, delta);
      UpdateCameraFPS(&player);

      BebisUpdate(&bebis, player.body.position, delta);

      BeginDrawing();
      ClearBackground(BLACK);

      DrawFPS(15, 15);

      BeginMode3D(player.camera);

      DrawBillboard(player.camera, bebis.texture, bebis.position, 2.0f, WHITE);
      DrawGrid(250, 1.0f);

      EndMode3D();
      EndDrawing();
    case GameOver:
      break;
    }
  }

  UnloadTexture(bebis.texture);
  CloseWindow();
  return 0;
}

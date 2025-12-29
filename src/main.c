#include "config.h"
#include <raylib.h>

static void DrawLevel(void);

int main(void) {
  SetConfigFlags(FLAG_VSYNC_HINT);
  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    EndDrawing();
  }

  return 0;
}

static void DrawLevel(void) {
  const int floorExtent = 25;
  const float tileSize = 5.0f;
  const Color tileColor1 = (Color){150, 200, 200, 255};

  for (int y = -floorExtent; y < floorExtent; y++) {
    for (int x = -floorExtent; x < floorExtent; x++) {
      DrawPlane((Vector3){x * tileSize, 0.0f, y * tileSize},
                (Vector2){-tileSize, tileSize}, tileColor1);
    }
  }

  const Vector3 towerSize = (Vector3){16.0f, 32.0f, 16.0f};
  const Color towerColor = (Color){150, 200, 200, 255};

  Vector3 towerPos = (Vector3){16.0f, 16.0f, 16.0f};
  DrawCubeV(towerPos, towerSize, towerColor);
  DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

  towerPos.x *= -1;
  DrawCubeV(towerPos, towerSize, towerColor);
  DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

  towerPos.z *= -1;
  DrawCubeV(towerPos, towerSize, towerColor);
  DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

  towerPos.x *= -1;
  DrawCubeV(towerPos, towerSize, towerColor);
  DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

  DrawSphere((Vector3){300.0f, 300.0f, 0.0f}, 100.0f, (Color){255, 0, 0, 255});
}

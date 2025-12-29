#ifndef BEBIS_H
#define BEBIS_H

#include "player.h"
#include <raylib.h>

typedef struct {
  Vector3 position;
  Vector3 velocity;
  float base_speed;
  float run_speed;
  Texture2D texture;
} Bebis;

Bebis BebisInit(Vector3 position, float base_speed, float run_speed,
                const char *texture_path);
void BebisUpdate(Bebis *bebis, Body *body, float delta);

#endif // !BEBIS_H

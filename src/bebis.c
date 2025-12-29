#include "bebis.h"
#include <raylib.h>
#include <raymath.h>

Bebis BebisInit(Vector3 position, float base_speed, float run_speed,
                const char *texture_path) {

  Texture2D tex = LoadTexture(texture_path);
  Bebis bebis = {.position = position,
                 .base_speed = base_speed,
                 .run_speed = run_speed,
                 .texture = tex};
  return bebis;
}

void BebisUpdate(Bebis *bebis, Body *body, float delta) {
  Vector3 dir = Vector3Subtract(body->position, bebis->position);
  float distance = Vector3Length(dir);
  bebis->position.y = 1.0f;

  if (distance > 0.01f) {
    dir = Vector3Normalize(dir);
    float speed = (distance < 5.0f) ? bebis->base_speed : bebis->run_speed;

    Vector3 desiredVel = Vector3Scale(dir, speed);
    bebis->velocity = Vector3Lerp(bebis->velocity, desiredVel, 5.0f * delta);
    bebis->position =
        Vector3Add(bebis->position, Vector3Scale(bebis->velocity, delta));
  }
}

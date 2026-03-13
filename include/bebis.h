#ifndef BEBIS_H
#define BEBIS_H

#include <raylib.h>

typedef struct {
  Vector3 position;
  Vector3 velocity;
  float base_speed;
  float run_speed;
  Texture2D texture;
  Music sound;
} Bebis;

Bebis BebisInit(const Vector3 position, const float base_speed,
                const float run_speed, const char *texture_path,
                const char *sound_path);
void BebisUpdate(Bebis *bebis, const Vector3 position, const float delta);
void UpdateBebisSound(const Vector3 v1, const Vector3 v2, Music sound);

#endif // !BEBIS_H

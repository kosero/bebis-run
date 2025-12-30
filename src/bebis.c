#include "bebis.h"
#include <raylib.h>
#include <raymath.h>

Bebis BebisInit(const Vector3 position, const float base_speed,
                const float run_speed, const char *texture_path,
                const char *sound_path) {
  Texture2D tex = LoadTexture(texture_path);
  Music bebisMusic = LoadMusicStream(sound_path);
  PlayMusicStream(bebisMusic);
  SetMusicVolume(bebisMusic, 0.0f);

  Bebis bebis = {.position = position,
                 .base_speed = base_speed,
                 .run_speed = run_speed,
                 .texture = tex,
                 .sound = bebisMusic};
  return bebis;
}

void BebisUpdate(Bebis *bebis, const Vector3 position, const float delta) {
  Vector3 dir = Vector3Subtract(position, bebis->position);
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

void UpdateBebisSound(const Vector3 v1, const Vector3 v2, Music sound) {
  const float distance = Vector3Distance(v1, v2);

  const float minDist = 10.0f;
  const float maxDist = 30.0f;

  float volume;

  if (distance <= minDist) {
    volume = 1.0f;
  } else if (distance >= maxDist) {
    volume = 0.0f;
  } else {
    volume = 1.0f - (distance - minDist) / (maxDist - minDist);
  }

  float targetVolume = volume;
  static float currentVolume = 0.0f;

  currentVolume = Lerp(currentVolume, targetVolume, 0.05f);
  SetMusicVolume(sound, currentVolume);

  UpdateMusicStream(sound);
  SetMusicVolume(sound, volume);
}

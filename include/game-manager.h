#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

typedef enum GameState {
  MainMenu,
  GameLoop,
  GameOver,
} GameState;

extern GameState currentGameState;

#endif // !GAME_MANAGER_H

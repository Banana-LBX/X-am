#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "cimp.h"

typedef struct Player {
    float health;
    size_t speed;
    
    Vector2 pos;
    Vector2 velocity;

    float hurt_timer;

    float rotation;
    size_t radius;
    Color color;
} Player;

void MovePlayer(Player *p);

void UpdatePlayer(
    Player *p,
    List *enemies,
    List *attacks,
    float *shakeTimer,
    float *shakeIntensity,
    float *hitTimer
);

void DrawPlayer(Player player);

#endif

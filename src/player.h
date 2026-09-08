#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "cimp.h"

typedef struct {
    float health;
    size_t speed;
    size_t radius;
    Vector2 pos;
    Vector2 velocity;
    float hurt_timer;
    Color color;
} Player;

void MovePlayer(Player *p);

void UpdatePlayer(
    Player *p,
    List *attacks,
    float *shakeTimer,
    float *shakeIntensity,
    float *hitTimer
);

#endif

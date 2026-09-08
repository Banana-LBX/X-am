#ifndef ATTACK_H
#define ATTACK_H

#include "game.h"
#include "enemy.h"
#include "cimp.h"

typedef struct {
    size_t damage;
    float cooldown;
    float active_timer;
    Vector2 start, end;
    float thickness;
    Color color;
    bool active;
} Attack;

void LinearAttack(List *attacks, Enemy enemy);

void UpdateAttacks(
    List *attacks,
    float *shakeTimer,
    float *shakeIntensity
);

void DrawAttacks(const List *attacks);

#endif

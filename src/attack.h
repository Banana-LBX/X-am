#ifndef ATTACK_H
#define ATTACK_H

#include "game.h"
#include "enemy.h"
#include "cimp.h"

typedef struct {
    size_t damage;
    float cooldown;
    float active_timer;

    Vector2 pos;
    float rotation;
    float radius;

    float thickness;
    Color color;

    bool active;
    Type type;
} Attack;

void LinearAttack(List *attacks, Enemy enemy);
void SineAttack1(List *attacks, Player p);
void SineAttack2(List *attacks);

void UpdateAttacks(
    List *attacks,
    float *shakeTimer,
    float *shakeIntensity
);

void DrawAttacks(const List *attacks);

#endif

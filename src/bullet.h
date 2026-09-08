#ifndef BULLET_H
#define BULLET_H

#include "game.h"
#include "player.h"
#include "cimp.h"

typedef struct {
    float damage;
    size_t speed;
    size_t duration;
    Vector2 pos;
    float radius;
    float rotation;
    Color color;
} Bullet;

void Shoot(Player p, List *bullets, size_t count);
void UpdateBullets(List *bullets);
void DrawBullets(const List *bullets);

#endif

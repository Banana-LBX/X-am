#ifndef PROJECTILE_H
#define PROJECTILE_H

// Basically bullets but for enemies
#include "game.h"
#include "enemy.h"
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
} Projectile;

void LinearShoot(Enemy *enemy, Player p);

void UpdateProjectiles(Enemy *enemy);
void DrawProjectiles(const List *projs);

#endif

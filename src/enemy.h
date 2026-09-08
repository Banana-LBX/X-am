#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include "bullet.h"
#include "cimp.h"

typedef enum {
    LINEAR,
} EnemyType;

typedef struct {
    float health;
    size_t speed;
    Rectangle rect;
    EnemyType type;
    float hurt_timer;
    Color color;
} Enemy;

void UpdateEnemies(List *enemies, List *bullets);
void DrawEnemies(const List *enemies);

Vector2 GetSpawnLocation(Player player);
void SpawnLinear(List *enemies, size_t count, Player player);

#endif

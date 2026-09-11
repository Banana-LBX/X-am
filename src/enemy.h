#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include "bullet.h"
#include "cimp.h"

typedef enum {
    LINEAR,
    SINE,
} Type;

typedef struct {
    float health;
    size_t speed;
    Rectangle rect;

    Type type;

    float hurt_timer;

    float rotation;
    Color color;
    Vector2 eye_pos;
} Enemy;

void UpdateEnemies(List *enemies, List *bullets);
void DrawEnemies(const List *enemies, Player p);

Vector2 GetSpawnLocation(Player player);
void SpawnLinear(List *enemies, size_t count, Player player);
void SpawnSine(List *enemies, size_t count, Player player);

Vector2 MoveEye(Vector2 currentPos, Vector2 targetPos, Vector2 anchorPos, float speed, float minDst, float maxDst);

#endif

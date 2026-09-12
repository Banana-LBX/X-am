#ifndef ENEMY_H
#define ENEMY_H

#include "game.h"
#include "player.h"
#include "cimp.h"

typedef enum {
    LINEAR,
    SINE,
} Type;

typedef struct Enemy {
    float health;
    size_t speed;
    Rectangle rect;

    Type type;

    List projs;
    size_t shots;
    size_t max_shots;

    float hurt_timer;

    float charge_timer;
    float charge_duration;

    float shoot_timer;
    float shoot_speed;

    Vector2 shake_offset;
    Vector2 recoil_velocity;

    float rotation;
    Color color;
    Vector2 eye_pos;
} Enemy;

void CheckEnemiesCollision(List *enemies, List *bullets);
void ShakeEnemy(Enemy *enemy);
void DrawEnemies(const List *enemies, Player p);

Vector2 GetSpawnLocation(Player player);
void SpawnLinear(List *enemies, size_t count, Player player);
void SpawnSine(List *enemies, size_t count, Player player);

Vector2 MoveEye(Vector2 currentPos, Vector2 targetPos, Vector2 anchorPos, float speed, float minDst, float maxDst);

#endif

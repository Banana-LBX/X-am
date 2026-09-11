#include "enemy.h"
#include "game.h"
#include <raylib.h>

void UpdateEnemies(List *enemies, List *bullets) {
    float dt = GetFrameTime();

    for (int i = (int)enemies->count - 1; i >= 0; i--) {
        Enemy *enemy = list_get(Enemy, enemies, i);

        bool dead = false;

        // Check bullets against this enemy
        for (int j = (int)bullets->count - 1; j >= 0; j--) {
            Bullet *bullet = list_get(Bullet, bullets, j);

            if (CheckCollisionCircleRec(
                bullet->pos,
                bullet->radius,
                enemy->rect
            )) {
                enemy->health -= bullet->damage;
                enemy->hurt_timer = 0.1f;

                list_remove(bullets, j);

                if (enemy->health <= 0.0f) {
                    dead = true;
                    break;
                }
            }
        }

        // Do not use enemy after it has been removed
        if (dead) {
            list_remove(enemies, i);
            continue;
        }

        // Move enemy toward the player
        Vector2 enemyCenter = {
            enemy->rect.x + enemy->rect.width / 2.0f,
            enemy->rect.y + enemy->rect.height / 2.0f
        };

        /*
         * This function currently only receives the bullet list,
         * so enemy movement toward the player is not performed here.
         */

        // Update hurt timer
        if (enemy->hurt_timer > 0.0f) {
            enemy->hurt_timer -= dt;

            if (enemy->hurt_timer < 0.0f)
                enemy->hurt_timer = 0.0f;

            enemy->color = WHITE;
        } else {
            switch (enemy->type) {
                case LINEAR:
                    enemy->color = LINEAR_COLOR;
                    break;

                case SINE:
                    enemy->color = SINE_COLOR;
                    break;

                default:
                    break;
            }
        }

        (void)enemyCenter;
    }
}

void DrawEnemies(const List *enemies, Player p) {
    for (size_t i = 0; i < enemies->count; i++) {
        Enemy *enemy = list_get(Enemy, enemies, i);

        switch (enemy->type) {
            case LINEAR:
                DrawRectangleLinesEx(
                    enemy->rect,
                    ENEMY_THICKNESS,
                    enemy->color
                );
                break;

            case SINE:
                for (size_t thick = 0; thick < 4; thick++) {
                    DrawCircleLines(
                        enemy->rect.x,
                        enemy->rect.y,
                        enemy->rect.width-thick,
                        enemy->color
                    );

                    enemy->eye_pos = MoveEye(enemy->eye_pos, p.pos, (Vector2){enemy->rect.x, enemy->rect.y}, SINE_EYE_SPEED, 1, enemy->rect.width/SINE_EYE_MAX_DIST);
                    DrawCircle(enemy->eye_pos.x, enemy->eye_pos.y, SINE_EYE_RADIUS, enemy->color);
                }
                break;

            default:
                break;
        }
    }
}

Vector2 GetSpawnLocation(Player player) {
    Vector2 pos = {
        GetRandomValue(20, WIDTH - 20),
        GetRandomValue(20, HEIGHT - 20)
    };

    while (Vector2Distance(pos, player.pos) < 100.0f) {
        pos = (Vector2){
            GetRandomValue(20, WIDTH - 20),
            GetRandomValue(20, HEIGHT - 20)
        };
    }

    return pos;
}

void SpawnLinear(List *enemies, size_t count, Player player) {
    for (size_t i = 0; i < count; i++) {
        Vector2 pos = GetSpawnLocation(player);

        Enemy enemy = {
            .health = LINEAR_HEALTH,
            .speed = LINEARS_SPEED,
            .rect = (Rectangle){
                pos.x,
                pos.y,
                LINEAR_SIZE,
                LINEAR_SIZE
            },
            .type = LINEAR,
            .hurt_timer = 0.0f,
            .rotation = 0.0f,
            .color = LINEAR_COLOR,
        };

        list_push(enemies, enemy);
    }
}

void SpawnSine(List *enemies, size_t count, Player player) {
    for (size_t i = 0; i < count; i++) {
        Vector2 pos = GetSpawnLocation(player);

        Enemy enemy = {
            .health = SINE_HEALTH,
            .speed = SINE_SPEED,
            .rect = (Rectangle){
                pos.x,
                pos.y,
                SINE_SIZE,
                SINE_SIZE
            },
            .type = SINE,
            .hurt_timer = 0.0f,
            .rotation = 0.0f,
            .color = SINE_COLOR,
            .eye_pos = (Vector2){enemy.rect.x, enemy.rect.y}
        };

        list_push(enemies, enemy);
    }
}

Vector2 MoveEye(Vector2 currentPos, Vector2 targetPos, Vector2 anchorPos, float speed, float minDst, float maxDst) {
    Vector2 nextPos = Vector2MoveTowards(currentPos, targetPos, speed * GetFrameTime());
    
    Vector2 offset = Vector2Subtract(nextPos, anchorPos);
    Vector2 clampedOffset = Vector2ClampValue(offset, minDst, maxDst);
    
    return Vector2Add(anchorPos, clampedOffset);
}

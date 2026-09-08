#include "enemy.h"

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
            enemy->color = YELLOW;
        }

        (void)enemyCenter;
    }
}

void DrawEnemies(const List *enemies) {
    for (size_t i = 0; i < enemies->count; i++) {
        Enemy *enemy = list_get(Enemy, enemies, i);

        DrawRectangleLinesEx(
            enemy->rect,
            ENEMY_THICKNESS,
            enemy->color
        );
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
            .color = YELLOW,
        };

        list_push(enemies, enemy);
    }
}

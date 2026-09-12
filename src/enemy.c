#include "enemy.h"
#include "game.h"
#include "bullet.h"
#include "projectile.h"
#include <raylib.h>

void CheckEnemiesCollision(List *enemies, List *bullets) {
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
    }
}

void ShakeEnemy(Enemy *enemy) {
    if (enemy->charge_timer <= 0.0f) {
        enemy->shake_offset = (Vector2){0.0f, 0.0f};
        return;
    }

    float progress =
        enemy->charge_duration / enemy->charge_timer / 5;

    float shake = progress;

    enemy->shake_offset = (Vector2){
        GetRandomValue(-100, 100) / 100.0f * shake,
        GetRandomValue(-100, 100) / 100.0f * shake
    };

    enemy->color = ColorBrightness(enemy->color, progress);
}

void DrawEnemies(const List *enemies, Player p) {
    for (size_t i = 0; i < enemies->count; i++) {
        Enemy *enemy = list_get(Enemy, enemies, i);

        Vector2 drawPos;
        Vector2 eyeDrawPos;

        switch (enemy->type) {
            case LINEAR: ;
                drawPos = Vector2Add(
                    (Vector2){enemy->rect.x, enemy->rect.y},
                    enemy->shake_offset
                );

                DrawPolyLinesEx(
                    drawPos,
                    4,
                    enemy->rect.width / 1.5,
                    enemy->rotation,
                    ENEMY_THICKNESS,
                    enemy->color
                );

                enemy->eye_pos = MoveEye(
                    enemy->eye_pos,
                    p.pos,
                    (Vector2){enemy->rect.x, enemy->rect.y},
                    LINEAR_EYE_SPEED,
                    1,
                    enemy->rect.width / LINEAR_EYE_MAX_DIST
                );

                eyeDrawPos = Vector2Add(
                    enemy->eye_pos,
                    enemy->shake_offset
                );

                DrawCircle(
                    eyeDrawPos.x,
                    eyeDrawPos.y,
                    LINEAR_EYE_RADIUS,
                    enemy->color
                );

                break;

        case SINE: ;
            drawPos = Vector2Add(
                (Vector2){enemy->rect.x, enemy->rect.y},
                enemy->shake_offset
            );

            for (size_t thick = 0; thick < 4; thick++) {
                DrawCircleLines(
                    drawPos.x,
                    drawPos.y,
                    enemy->rect.width - thick,
                    enemy->color
                );
            }

            enemy->eye_pos = MoveEye(
                enemy->eye_pos,
                p.pos,
                (Vector2){enemy->rect.x, enemy->rect.y},
                SINE_EYE_SPEED,
                1,
                enemy->rect.width / SINE_EYE_MAX_DIST
            );

            eyeDrawPos = Vector2Add(
                enemy->eye_pos,
                enemy->shake_offset
            );

            DrawCircle(
                eyeDrawPos.x,
                eyeDrawPos.y,
                SINE_EYE_RADIUS,
                enemy->color
            );

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
            .speed = LINEAR_SPEED,
            .rect = (Rectangle){
                pos.x,
                pos.y,
                LINEAR_SIZE,
                LINEAR_SIZE
            },
            .type = LINEAR,
            .projs = list_new(Projectile),
            .shots = LINEAR_SHOTS,
            .max_shots = LINEAR_SHOTS,

            .hurt_timer = 0.0f,

            .charge_timer = LINEAR_CHARGE_TIME,
            .charge_duration = LINEAR_CHARGE_TIME,

            .shoot_timer = 0.0f,
            .shoot_speed = LINEAR_SHOOT_SPEED,

            .shake_offset = (Vector2){0.0f, 0.0f},
            .recoil_velocity = (Vector2){0.0f, 0.0f},

            .rotation = 0.0f,
            .color = LINEAR_COLOR,
            .eye_pos = (Vector2){pos.x, pos.y}
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
            .projs = list_new(Projectile),

            .hurt_timer = 0.0f,

            .charge_timer = SINE_CHARGE_TIME,
            .charge_duration = SINE_CHARGE_TIME,

            .shoot_timer = 0.0f,
            .shoot_speed = SINE_SHOOT_SPEED,

            .shake_offset = (Vector2){0.0f, 0.0f},
            .recoil_velocity = (Vector2){0.0f, 0.0f},

            .rotation = 0.0f,
            .color = SINE_COLOR,
            .eye_pos = (Vector2){pos.x, pos.y}
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

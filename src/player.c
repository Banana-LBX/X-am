#include "player.h"
#include "projectile.h"
#include "attack.h"
#include "cimp.h"
#include <raylib.h>

void MovePlayer(Player *p) {
    Vector2 direction = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_D)) direction.x += 1.0f;

    float dt = GetFrameTime();

    // Movement tuning
    float acceleration = 2000.0f;
    float deceleration = 1000.0f;
    float maxSpeed = (float)p->speed * 20.0f;

    // Get the desired movement direction
    if (direction.x != 0.0f || direction.y != 0.0f) {
        direction = Vector2Normalize(direction);

        // Accelerate toward the desired direction
        p->velocity.x += direction.x * acceleration * dt;
        p->velocity.y += direction.y * acceleration * dt;

        // Prevent the player from exceeding max speed
        float velocityLength = Vector2Length(p->velocity);

        if (velocityLength > maxSpeed) {
            p->velocity = Vector2Scale(
                Vector2Normalize(p->velocity),
                maxSpeed
            );
        }
    } else {
        // No input: gradually slow down
        float velocityLength = Vector2Length(p->velocity);

        if (velocityLength > 0.0f) {
            float newSpeed = velocityLength - deceleration * dt;

            if (newSpeed < 0.0f)
                newSpeed = 0.0f;

            if (newSpeed == 0.0f) {
                p->velocity = (Vector2){ 0.0f, 0.0f };
            } else {
                p->velocity = Vector2Scale(
                    Vector2Normalize(p->velocity),
                    newSpeed
                );
            }
        }
    }

    // Apply velocity
    p->pos.x += p->velocity.x * dt;
    p->pos.y += p->velocity.y * dt;

    // Keep the player inside the screen
    if (p->pos.x > WIDTH)   p->pos.x = 0;
    if (p->pos.x < 0)       p->pos.x = WIDTH;
    if (p->pos.y > HEIGHT)  p->pos.y = 0;
    if (p->pos.y < 0)       p->pos.y = HEIGHT;
}

void UpdatePlayer(
    Player *p,
    List *enemies,
    List *attacks,
    float *shakeTimer,
    float *shakeIntensity,
    float *hitTimer
) {
    float dt = GetFrameTime();

    // Update invincibility timer
    if (p->hurt_timer > 0.0f) {
        p->hurt_timer -= dt;

        if (p->hurt_timer < 0.0f)
            p->hurt_timer = 0.0f;
    }

    for (size_t i = 0; i < attacks->count; i++) {
        Attack *attack = list_get(Attack, attacks, i);

        if (
            p->hurt_timer <= 0.0f &&
            attack->active
        ) {
            switch (attack->type) {
                case LINEAR: {
                    Vector2 direction = {
                        cosf(attack->rotation),
                        sinf(attack->rotation)
                    };

                    float length = 3000.0f;

                    Vector2 start = Vector2Subtract(
                        attack->pos,
                        Vector2Scale(direction, length)
                    );

                    Vector2 end = Vector2Add(
                        attack->pos,
                        Vector2Scale(direction, length)
                    );

                    if (CheckCollisionCircleLine(
                        p->pos,
                        p->radius,
                        start,
                        end
                    )) {
                        p->health -= (float)attack->damage;

                        p->hurt_timer = INVINCI_TIME;

                        *shakeTimer = 0.20f;
                        *shakeIntensity = 50.0f;

                        *hitTimer = 1.5f;

                        break;
                    }

                    break;
                }

                case SINE:
                    if (CheckCollisionCircles(
                        p->pos,
                        p->radius,
                        attack->pos,
                        attack->radius
                    )) {
                        p->health -= (float)attack->damage;

                        p->hurt_timer = INVINCI_TIME;

                        *shakeTimer = 0.20f;
                        *shakeIntensity = 50.0f;

                        *hitTimer = 1.5f;

                        break;
                    }

                    break;

                default:
                    break;
            }
        }
    }

    for (size_t i = 0; i < enemies->count; i++) {
        Enemy *enemy = list_get(Enemy, enemies, i);

        for (size_t j = 0; j < enemy->projs.count; j++) {
            Projectile *proj = list_get(Projectile, &enemy->projs, j);

            if (CheckCollisionCircles(
                p->pos,
                p->radius,
                proj->pos,
                proj->radius
            )) {
                p->health -= (float)proj->damage;

                p->hurt_timer = INVINCI_TIME;

                *shakeTimer = 0.20f;
                *shakeIntensity = 50.0f;

                *hitTimer = 1.5f;
            }
        }
    }

    // Flash red while invincible
    if (p->hurt_timer > 0.0f)
        p->color = RED;
    else
        p->color = WHITE;
}

void DrawPlayer(Player player) {
    float r = player.radius;
    float rotation = player.rotation * RAD2DEG;

    DrawPolyLinesEx(
        player.pos,
        3,
        r,
        rotation,
        2.0f,
        player.color
    );

    DrawPoly(
        player.pos,
        3,
        r / 2.0f,
        rotation,
        player.color
    );
}

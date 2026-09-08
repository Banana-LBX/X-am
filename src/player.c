#include "player.h"
#include "attack.h"

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
    p->pos.x = Clamp(
        p->pos.x,
        (float)p->radius,
        WIDTH - (float)p->radius
    );

    p->pos.y = Clamp(
        p->pos.y,
        (float)p->radius,
        HEIGHT - (float)p->radius
    );
}

void UpdatePlayer(
    Player *p,
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
            attack->active &&
            CheckCollisionCircleLine(
                p->pos,
                p->radius,
                attack->start,
                attack->end
            )
        ) {
            p->health -= (float)attack->damage;

            // Give the player 0.2 seconds of invincibility
            p->hurt_timer = INVINCI_TIME;

            // Hit screen shake
            *shakeTimer = 0.20f;
            *shakeIntensity = 50.0f;

            *hitTimer = 1.5f;

            // Only allow one hit per frame
            break;
        }
    }

    // Flash red while invincible
    if (p->hurt_timer > 0.0f)
        p->color = RED;
    else
        p->color = WHITE;
}

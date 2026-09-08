#include "attack.h"

void LinearAttack(List *attacks, Enemy enemy) {
    float angle = (float)GetRandomValue(0, 36000) / 100.0f;
    angle *= DEG2RAD;

    Vector2 center = {
        enemy.rect.x + enemy.rect.width / 2.0f,
        enemy.rect.y + enemy.rect.height / 2.0f
    };

    Vector2 direction = {
        cosf(angle),
        sinf(angle)
    };

    float length = 3000.0f;

    Attack attack = {
        .damage = LINEAR_DAMAGE,
        .cooldown = LINEAR_COOLDOWN,
        .active_timer = LINEAR_ACTIVE,
        .start = Vector2Subtract(
            center,
            Vector2Scale(direction, length)
        ),
        .end = Vector2Add(
            center,
            Vector2Scale(direction, length)
        ),
        .thickness = ATTACK_THICKNESS,
        .color = RED,
        .active = false
    };

    list_push(attacks, attack);
}

void UpdateAttacks(List *attacks, float *shakeTimer, float *shakeIntensity) {
    for (int i = (int)attacks->count - 1; i >= 0; i--) {
        Attack *attack = list_get(Attack, attacks, i);

        // Charge up attack
        if (attack->cooldown > 0.0f) {
            attack->cooldown -= GetFrameTime();

            if (attack->cooldown < 0.0f)
                attack->cooldown = 0.0f;
        }

        // Attack active
        else {
            if (!attack->active) {
                attack->active = true;

                // Screen shake
                *shakeTimer = 0.15f;
                *shakeIntensity = 10.0f;
            }

            attack->active_timer -= GetFrameTime();

            // Delete attack
            if (attack->active_timer <= 0.0f) {
                list_remove(attacks, i);
            }
        }
    }
}

void DrawAttacks(const List *attacks) {
    for (size_t i = 0; i < attacks->count; i++) {
        Attack *attack = list_get(Attack, attacks, i);

        // Charge up attack
        if (attack->cooldown > 0.0f) {
            float thickness =
                attack->thickness *
                attack->cooldown *
                4.0f;

            DrawLineEx(
                attack->start,
                attack->end,
                thickness,
                ColorAlpha(attack->color, 0.5f)
            );
        }

        // Attack active
        else {
            // Draw full thickness
            DrawLineEx(
                attack->start,
                attack->end,
                attack->thickness,
                WHITE
            );
        }
    }
}

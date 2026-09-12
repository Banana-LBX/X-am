#include "attack.h"
#include "cimp.h"
#include "game.h"
#include <raylib.h>

void LinearAttack(List *attacks, Enemy enemy) {
    float angle = (float)GetRandomValue(0, 36000) / 100.0f;
    angle *= DEG2RAD;

    Vector2 center = {
        enemy.rect.x + enemy.rect.width / 2.0f,
        enemy.rect.y + enemy.rect.height / 2.0f
    };

    Attack attack = {
        .damage = LINEAR_DAMAGE,
        .cooldown = LINEAR_COOLDOWN,
        .active_timer = LINEAR_ACTIVE,
        .pos = center,
        .rotation = angle,
        .thickness = ATTACK_THICKNESS,
        .color = LINEAR_COLOR,
        .active = false
    };

    list_push(attacks, attack);
}

void SineAttack1(List *attacks, Player p) {
    Vector2 pos = p.pos;

    for (size_t i = 0; i < 8; i++) {
        Attack attack = {
            .type = SINE,
            .damage = SINE_DAMAGE/2,
            .cooldown = SINE_COOLDOWN + i * 0.1f,
            .active_timer = SINE_ACTIVE,
            .pos = pos,
            .rotation = 0.0f,
            .radius = 10.0f + i * 20.0f,
            .thickness = ATTACK_THICKNESS,
            .color = SINE_COLOR,
            .active = false
        };

        list_push(attacks, attack);
    }
}

void SineAttack2(List *attacks) {
    for (size_t i = 0; i < 8; i++) {
        Vector2 pos = (Vector2){GetRandomValue(0, WIDTH), GetRandomValue(0, HEIGHT)};

        Attack attack = {
            .type = SINE,
            .damage = SINE_DAMAGE,
            .cooldown = SINE_COOLDOWN + 0.5f,
            .active_timer = SINE_ACTIVE,
            .pos = pos,
            .rotation = 0.0f,
            .radius = 80.0f,
            .thickness = ATTACK_THICKNESS,
            .color = PURPLE,
            .active = false
        };

        list_push(attacks, attack);
    }
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

        float thickness;
        Color color;

        // Charge up attack
        if (attack->cooldown > 0.0f) {
            thickness = attack->thickness * attack->cooldown * 4.0f;
            color = ColorAlpha(attack->color, 0.5f);
        }
        // Attack active
        else {
            // Full thickness
            thickness = attack->thickness;
            color = WHITE;
        }

        switch (attack->type) {
            case LINEAR: ;
                Vector2 direction = {
                    cosf(attack->rotation),
                    sinf(attack->rotation)
                };

                float length = 3000.0f;

                DrawLineEx(
                    Vector2Subtract(
                        attack->pos,
                        Vector2Scale(direction, length)
                        ),
                    Vector2Add(
                        attack->pos,
                        Vector2Scale(direction, length)
                        ),
                    thickness,
                    color
                );
                break;
            case SINE:
                for (size_t i = 0; i < thickness; i++) {
                    DrawCircleLinesV(
                        attack->pos,
                        attack->radius-i,
                        color
                    );
                }
                break;
            default:
                break;
        }
    }
}

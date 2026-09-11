#include "bullet.h"
#include <math.h>

void Shoot(Player p, List *bullets, size_t count) {
    Vector2 mouse = GetMousePosition();

    Vector2 direction = Vector2Subtract(mouse, p.pos);

    if (Vector2LengthSqr(direction) <= 0.0f)
        return;

    float baseRotation =
        atan2f(direction.y, direction.x) * RAD2DEG;

    for (size_t i = 0; i < count; i++) {
        float rotation =
            baseRotation +
            ((float)i - (float)(count - 1) / 2.0f) * BULLET_GAP;

        Bullet bullet = {
            .damage = 30.0f,
            .speed = 20,
            .duration = 80,
            .pos = p.pos,
            .radius = BULLET_RADIUS,
            .rotation = rotation,
            .color = WHITE,
        };

        list_push(bullets, bullet);
    }
}


void UpdateBullets(List *bullets) {
    float dt = GetFrameTime();

    for (int i = (int)bullets->count - 1; i >= 0; i--) {
        Bullet *bullet = list_get(Bullet, bullets, i);

        float angle = bullet->rotation * DEG2RAD;

        Vector2 forward = {
            cosf(angle),
            sinf(angle)
        };

        bullet->pos.x += forward.x * (float)bullet->speed * 60.0f * dt;
        bullet->pos.y += forward.y * (float)bullet->speed * 60.0f * dt;

        if (bullet->duration > 0)
            bullet->duration--;

        // Remove expired bullets
        if (
            bullet->duration == 0 ||
            bullet->pos.x < -bullet->radius ||
            bullet->pos.x > WIDTH + bullet->radius ||
            bullet->pos.y < -bullet->radius ||
            bullet->pos.y > HEIGHT + bullet->radius
        ) {
            list_remove(bullets, i);
        }
    }
}

void DrawBullets(const List *bullets) {
    for (size_t i = 0; i < bullets->count; i++) {
        Bullet *bullet = list_get(Bullet, bullets, i);

        if (bullet->duration > 0) {
            DrawCircleLinesV(
                bullet->pos,
                bullet->radius,
                bullet->color
            );
        }
    }
}

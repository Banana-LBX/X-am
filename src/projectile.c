#include "projectile.h"
#include "game.h"
#include <raylib.h>

void LinearShoot(Enemy *enemy, Player p) {
    Vector2 enemyCenter = {
        enemy->rect.x,
        enemy->rect.y
    };

    Vector2 direction =
        Vector2Subtract(p.pos, enemyCenter);

    if (Vector2LengthSqr(direction) <= 0.0f)
        return;

    float rotation =
        atan2f(direction.y, direction.x) * RAD2DEG;

    Projectile proj = {
        .damage = LINEAR_BULLET_DAMAGE,
        .speed = LINEAR_BULLET_SPEED,
        .duration = LINEAR_BULLET_DURATION,
        .pos = enemyCenter,
        .radius = BULLET_RADIUS,
        .rotation = rotation,
        .color = enemy->color
    };

    list_push(&enemy->projs, proj);
}

void UpdateProjectiles(Enemy *enemy) {
    float dt = GetFrameTime();

    for (int i = (int)enemy->projs.count - 1; i >= 0; i--) {
        Projectile *proj =
            list_get(Projectile, &enemy->projs, i);

        float angle = proj->rotation * DEG2RAD;

        Vector2 forward = {
            cosf(angle),
            sinf(angle)
        };

        proj->pos.x +=
            forward.x * (float)proj->speed * 60.0f * dt;

        proj->pos.y +=
            forward.y * (float)proj->speed * 60.0f * dt;

        if (proj->duration > 0)
            proj->duration--;

        // Remove expired bullets
        if (
            proj->duration == 0 ||
            proj->pos.x < -proj->radius ||
            proj->pos.x > WIDTH + proj->radius ||
            proj->pos.y < -proj->radius ||
            proj->pos.y > HEIGHT + proj->radius
        ) {
            list_remove(&enemy->projs, i);
        }
    }
}

void DrawProjectiles(const List *projs) {
    for (size_t i = 0; i < projs->count; i++) {
        const Projectile *proj =
            list_get(Projectile, projs, i);

        DrawCircleLinesV(
            proj->pos,
            proj->radius,
            proj->color
        );
    }
}

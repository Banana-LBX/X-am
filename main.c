#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#define CIMP_IMPLEMENTATION
#include "cimp.h"

#define WIDTH 1900
#define HEIGHT 1000

#define PLAYER_SPEED 5
#define PLAYER_RADIUS 16

#define BULLET_RADIUS 5
#define BULLET_GAP 20 // the angle gap in degrees of multiple bullets shot at once

#define ENEMY_THICKNESS 3.0f

#define ATTACK_THICKNESS 5.0f

#define MAX_ATTACKS WIDTH

typedef struct {
    size_t health;
    size_t speed;
    size_t radius;
    Vector2 pos;
    Color color;
} Player;

typedef struct {
    size_t damage;
    size_t speed;
    size_t duration;
    Vector2 pos;
    float radius;
    float rotation;
    Color color;
} Bullet;

typedef enum {
    LINEAR,
} EnemyType;

typedef struct {
    size_t health;
    size_t speed;
    Rectangle rect;
    EnemyType type;
    Color color;
} Enemy;

typedef struct {
    size_t damage;
    float cooldown;
    float active_timer;
    Vector2 start, end;
    float thickness;
    Color color;
    bool active;
} Attack;

void MovePlayer(Player *p);

void Shoot(Player p, List *bullets, size_t count);
void UpdateBullets(List *bullets);
void DrawBullets(const List *bullets);

Vector2 GetSpawnLocation(Player player);
void SpawnLinear(List *enemies, size_t count, Player player);
void DrawEnemies(List *enemies);

void LinearAttack(List *attacks, Enemy enemy);
void UpdateAttacks(List *attacks);

int main(void) {
    InitWindow(WIDTH, HEIGHT, "X-am");
    SetTargetFPS(60);

    Player player = {
        .health = 100,
        .speed = PLAYER_SPEED,
        .radius = PLAYER_RADIUS,
        .pos = {
            (float)WIDTH / 2.0f,
            (float)HEIGHT / 2.0f
        },
        .color = WHITE
    };

    List bullets = list_new(Bullet);
    List enemies = list_new(Enemy);
    List attacks = list_new(Attack);

    SpawnLinear(&enemies, 5, player);

    while (!WindowShouldClose()) {
        MovePlayer(&player);

        if (IsKeyPressed(KEY_SPACE))
            Shoot(player, &bullets, 5);
        UpdateBullets(&bullets);

        if (IsKeyPressed(KEY_O)) {
            for (size_t i = 0; i < enemies.count; i++) {
                Enemy *enemy = list_get(Enemy, &enemies, i);
                LinearAttack(&attacks, *enemy);
            }
        }

        // Rendering
        BeginDrawing();
        ClearBackground(BLACK);
        // Player
        DrawCircleLinesV(
            player.pos,
            player.radius,
            player.color
        );
        DrawCircleV(
            player.pos,
            player.radius - (float)player.radius / 3.5f,
            player.color
        );
        // Bullets
        DrawBullets(&bullets);
        // Enemies
        DrawEnemies(&enemies);
        // Attacks
        UpdateAttacks(&attacks);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void MovePlayer(Player *p) {
    Vector2 direction = { 0.0f, 0.0f };

    if (IsKeyDown(KEY_W)) direction.y -= 1.0f;
    if (IsKeyDown(KEY_S)) direction.y += 1.0f;
    if (IsKeyDown(KEY_A)) direction.x -= 1.0f;
    if (IsKeyDown(KEY_D)) direction.x += 1.0f;

    if (direction.x != 0.0f || direction.y != 0.0f) {
        direction = Vector2Normalize(direction);

        p->pos.x += direction.x * p->speed;
        p->pos.y += direction.y * p->speed;
    }
}

void Shoot(Player p, List *bullets, size_t count) {
    Vector2 mouse = GetMousePosition();

    Vector2 direction = Vector2Subtract(mouse, p.pos);

    float baseRotation =
        atan2f(direction.y, direction.x) * RAD2DEG;

    for (size_t i = 0; i < count; i++) {
        float rotation =
            baseRotation +
            (i - (count - 1) / 2.0f) * BULLET_GAP;

        Bullet bullet = {
            .damage = 30,
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

void UpdateBullets(List *bullets) { for (size_t i = 0; i < bullets->count; i++) { Bullet *bullet = list_get(Bullet, bullets, i);
        float angle = bullet->rotation * DEG2RAD;
        Vector2 forward = {
            cosf(angle),
            sinf(angle)
        };

        bullet->pos.x += forward.x * bullet->speed;
        bullet->pos.y += forward.y * bullet->speed;
    }
}

void DrawBullets(const List *bullets) {
    for (size_t i = 0; i < bullets->count; i++) {
        Bullet *bullet = list_get(Bullet, bullets, i);

        if (bullet->duration > 0) {
            DrawCircleLinesV(bullet->pos, bullet->radius, bullet->color);
            bullet->duration--;
        }
    }
}

Vector2 GetSpawnLocation(Player player) {
    Vector2 pos = {
        GetRandomValue(0, WIDTH - 20),
        GetRandomValue(0, HEIGHT - 20)
    };

    while (Vector2Distance(pos, player.pos) < 100.0f) {
        pos = (Vector2){
            GetRandomValue(0, WIDTH - 20),
            GetRandomValue(0, HEIGHT - 20)
        };
    }

    return pos;
}

void SpawnLinear(List *enemies, size_t count, Player player) {
    for (size_t i = 0; i < count; i++) {
        Vector2 pos = GetSpawnLocation(player);

        Enemy enemy = {
            .health = 100,
            .speed = 4,
            .rect = (Rectangle){
                pos.x,
                pos.y,
                20.0f,
                20.0f
            },
            .type = LINEAR,
            .color = YELLOW,
        };

        list_push(enemies, enemy);
    }
}

void DrawEnemies(List *enemies) {
    for (size_t i = 0; i < enemies->count; i++) {
        Enemy *enemy = list_get(Enemy, enemies, i);

        DrawRectangleLinesEx(enemy->rect, ENEMY_THICKNESS, enemy->color);
    }
}

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
        .damage = 30,
        .cooldown = 1.5f,        // 1.5 seconds warning line indicator
        .active_timer = 0.4f,    // 0.4 seconds flash duration when firing
        .start = Vector2Subtract(center, Vector2Scale(direction, length)),
        .end = Vector2Add(center, Vector2Scale(direction, length)),
        .thickness = ATTACK_THICKNESS,
        .color = RED,
        .active = false
    };

    list_push(attacks, attack);
}

void UpdateAttacks(List *attacks) {
    for (int i = (int)attacks->count - 1; i >= 0; i--) {
        Attack *attack = list_get(Attack, attacks, i);

        // Charge up attack
        if (attack->cooldown > 0.0f) {
            attack->cooldown -= GetFrameTime();
            
            // Warning line that gets narrower
            DrawLineEx(attack->start, attack->end, attack->thickness*attack->cooldown*4, ColorAlpha(attack->color, 0.5f));
        } 

        // Attack active
        else {
            attack->active = true;
            attack->active_timer -= GetFrameTime();

            // Draw full thickness
            DrawLineEx(attack->start, attack->end, attack->thickness, WHITE);

            // Delete attack
            if (attack->active_timer <= 0.0f) {
                list_remove(attacks, i);
            }
        }
    }
}

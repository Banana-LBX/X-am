#include <raylib.h>

#define CIMP_IMPLEMENTATION
#include "cimp.h"

#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "attack.h"
#include "projectile.h"
#include "screen_shake.h"
#include "shader.h"

int main(void) {
    InitWindow(WIDTH, HEIGHT, "X-am");
    SetTargetFPS(60);

    Camera2D camera = {
        .offset = {
            WIDTH / 2.0f,
            HEIGHT / 2.0f
        },
        .target = {
            WIDTH / 2.0f,
            HEIGHT / 2.0f
        },
        .rotation = 0.0f,
        .zoom = 1.0f
    };

    RenderTexture2D target =
        LoadRenderTexture(WIDTH, HEIGHT);

    GameShader shader =
        LoadGameShader("src/crt_bloom.fs");

    Player player = {
        .health = 100.0f,
        .speed = PLAYER_SPEED,
        .pos = {
            WIDTH / 2.0f,
            HEIGHT / 2.0f
        },
        .velocity = {
            0.0f,
            0.0f
        },
        .rotation = 0.0f,
        .radius = PLAYER_RADIUS,
        .color = WHITE
    };

    List playerBullets = list_new(Bullet);
    List enemies = list_new(Enemy);
    List attacks = list_new(Attack);

    SpawnLinear(&enemies, 5, player);
    SpawnSine(&enemies, 5, player);

    float shakeTimer = 0.0f;
    float shakeIntensity = 0.0f;

    float hitTimer = 0.0f;

    while (!WindowShouldClose()) {
        /*
         * Update
         */

        float dt = GetFrameTime();

        // Player
        MovePlayer(&player);

        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        player.rotation = atan2f(
            mouseWorld.y - player.pos.y,
            mouseWorld.x - player.pos.x
        );

        UpdatePlayer(
            &player,
            &enemies,
            &attacks,

            &shakeTimer,
            &shakeIntensity,
            &hitTimer
        );

        if (hitTimer > 0.0f) {
            hitTimer -= GetFrameTime();

            if (hitTimer < 0.0f)
                hitTimer = 0.0f;
        }

        if (IsKeyPressed(KEY_SPACE))
            PlayerShoot(player, &playerBullets, 5);

        UpdateBullets(&playerBullets);

        // Enemies
        CheckEnemiesCollision(&enemies, &playerBullets);

        // Enemy attacks
        if (IsKeyPressed(KEY_O)) {
            for (size_t i = 0; i < enemies.count; i++) {
                Enemy *enemy =
                    list_get(Enemy, &enemies, i);

                switch (enemy->type) {
                    case LINEAR:
                        LinearAttack(&attacks, *enemy);
                        break;
                    case SINE:
                        if (GetRandomValue(0, 1) == 0) 
                            SineAttack1(&attacks, player);
                        else
                            SineAttack2(&attacks);
                        break;
                    default:
                        break;
                }
            }
        }

        // Enemy shooting
        if (IsKeyPressed(KEY_P)) {
            for (size_t i = 0; i < enemies.count; i++) {
                Enemy *enemy =
                    list_get(Enemy, &enemies, i);

                if (enemy->type == LINEAR) {
                    enemy->shots = enemy->max_shots;
                    enemy->charge_timer = enemy->charge_duration;
                    enemy->shoot_timer = 0.0f;
                    enemy->shake_offset =
                        (Vector2){0.0f, 0.0f};
                }
            }
        }

        for (size_t i = 0; i < enemies.count; i++) {
            Enemy *enemy =
                list_get(Enemy, &enemies, i);

            if (enemy->type != LINEAR)
                continue;

            // Charging
            if (enemy->charge_timer > 0.0f) {
                enemy->charge_timer -= dt;

                if (enemy->charge_timer < 0.0f)
                    enemy->charge_timer = 0.0f;

                ShakeEnemy(enemy);

                continue;
            }

            // No more shaking, charged
            enemy->shake_offset =
                (Vector2){0.0f, 0.0f};

            if (enemy->shots == 0)
                continue;

            // Wait between shots
            if (enemy->shoot_timer > 0.0f) {
                enemy->shoot_timer -= dt;

                if (enemy->shoot_timer < 0.0f)
                    enemy->shoot_timer = 0.0f;

                continue;
            }

            // Shoot
            LinearShoot(enemy, player);

            enemy->shots--;

            // Recoil
            Vector2 direction = Vector2Normalize(
                Vector2Subtract(
                    (Vector2){
                        enemy->rect.x,
                        enemy->rect.y
                    },
                    player.pos
                )
            );

            enemy->recoil_velocity =
                Vector2Scale(direction, 120.0f);

            // Time until next shot
            enemy->shoot_timer =
                enemy->shoot_speed;

            // Finish shooting
            if (enemy->shots == 0) {
                enemy->shoot_timer = 0.0f;
            }
        }

        // Enemy recoil
        for (size_t i = 0; i < enemies.count; i++) {
            Enemy *enemy =
                list_get(Enemy, &enemies, i);

            enemy->rect.x += enemy->recoil_velocity.x * dt;
            enemy->rect.y += enemy->recoil_velocity.y * dt;

            enemy->recoil_velocity =
                Vector2Scale(enemy->recoil_velocity, 0.85f);
            
            enemy->rect.x = Clamp(
                enemy->rect.x,
                (float)enemy->rect.width,
                WIDTH - (float)enemy->rect.width
            );

            enemy->rect.y = Clamp(
                enemy->rect.y,
                (float)enemy->rect.height,
                HEIGHT - (float)enemy->rect.height
            );
        }

        UpdateAttacks(
            &attacks,
            &shakeTimer,
            &shakeIntensity
        );

        for (size_t i = 0; i < enemies.count; i++) {
            UpdateProjectiles(list_get(Enemy, &enemies, i));
        }

        // Screen shake
        UpdateScreenShake(
            &camera,
            &shakeTimer,
            &shakeIntensity
        );

        /*
         * Render game to texture
         */

        BeginTextureMode(target);

        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawPlayer(player);
        DrawBullets(&playerBullets);

        DrawEnemies(&enemies, player);
        DrawAttacks(&attacks);
        for (size_t i = 0; i < enemies.count; i++) {
            DrawProjectiles(&list_get(Enemy, &enemies, i)->projs);
        }

        DrawText(
            TextFormat(
                "Health: %.0f",
                player.health
            ),
            10,
            10,
            30,
            WHITE
        );

        EndMode2D();

        EndTextureMode();

        /*
         * Post processing
         */

        UpdateGameShader(
            &shader,
            (float)GetTime(),
            hitTimer
        );

        BeginDrawing();

        ClearBackground(BLACK);

        BeginShaderMode(shader.shader);

        DrawTextureRec(
            target.texture,
            (Rectangle){
                0.0f,
                0.0f,
                (float)WIDTH,
                -(float)HEIGHT
            },
            (Vector2){ 0.0f, 0.0f },
            WHITE
        );

        EndShaderMode();

        EndDrawing();
    }

    /*
     * Cleanup
     */

    UnloadGameShader(&shader);

    UnloadRenderTexture(target);

    CloseWindow();

    return 0;
}

#include <raylib.h>

#define CIMP_IMPLEMENTATION
#include "cimp.h"

#include "game.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "attack.h"
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

    List bullets = list_new(Bullet);
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

        MovePlayer(&player);

        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        player.rotation = atan2f(
            mouseWorld.y - player.pos.y,
            mouseWorld.x - player.pos.x
        );

        UpdatePlayer(
            &player,
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
            Shoot(player, &bullets, 5);

        UpdateBullets(&bullets);

        UpdateEnemies(&enemies, &bullets);

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

        UpdateAttacks(
            &attacks,
            &shakeTimer,
            &shakeIntensity
        );

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
        DrawBullets(&bullets);
        DrawEnemies(&enemies, player);
        DrawAttacks(&attacks);

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

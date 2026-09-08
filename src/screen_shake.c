#include "game.h"
#include "screen_shake.h"

void UpdateScreenShake(Camera2D *camera, float *shakeTimer, float *shakeIntensity) {
    if (*shakeTimer > 0.0f) {
        *shakeTimer -= GetFrameTime();

        camera->offset.x =
            WIDTH / 2.0f +
            (float)GetRandomValue(
                -(int)(*shakeIntensity * 100.0f),
                (int)(*shakeIntensity * 100.0f)
            ) / 100.0f;

        camera->offset.y =
            HEIGHT / 2.0f +
            (float)GetRandomValue(
                -(int)(*shakeIntensity * 100.0f),
                (int)(*shakeIntensity * 100.0f)
            ) / 100.0f;

        // Gradually reduce shake intensity
        *shakeIntensity *= 0.90f;

        if (*shakeTimer <= 0.0f) {
            *shakeTimer = 0.0f;
            *shakeIntensity = 0.0f;
        }
    }
    else {
        camera->offset.x = WIDTH / 2.0f;
        camera->offset.y = HEIGHT / 2.0f;

        *shakeIntensity = 0.0f;
    }
}

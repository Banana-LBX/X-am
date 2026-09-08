#ifndef SCREEN_SHAKE_H
#define SCREEN_SHAKE_H

#include <raylib.h>

void UpdateScreenShake(
    Camera2D *camera,
    float *shakeTimer,
    float *shakeIntensity
);

#endif

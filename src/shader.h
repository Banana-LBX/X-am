#ifndef SHADER_H
#define SHADER_H

#include <raylib.h>

typedef struct {
    Shader shader;
    int timeLoc;
    int hitTimerLoc;
} GameShader;

GameShader LoadGameShader(const char *fragmentShader);

void UpdateGameShader(
    GameShader *shader,
    float time,
    float hitTimer
);

void UnloadGameShader(GameShader *shader);

#endif

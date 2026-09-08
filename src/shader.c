#include "game.h"
#include "shader.h"

GameShader LoadGameShader(const char *fragmentShader) {
    GameShader result = {
        .shader = LoadShader(0, fragmentShader)
    };

    result.timeLoc =
        GetShaderLocation(result.shader, "time");

    result.hitTimerLoc =
        GetShaderLocation(result.shader, "hitTimer");

    int resolutionLoc =
        GetShaderLocation(result.shader, "resolution");

    Vector2 resolution = {
        (float)WIDTH,
        (float)HEIGHT
    };

    SetShaderValue(
        result.shader,
        resolutionLoc,
        &resolution,
        SHADER_UNIFORM_VEC2
    );

    return result;
}

void UpdateGameShader(
    GameShader *shader,
    float time,
    float hitTimer
) {
    SetShaderValue(
        shader->shader,
        shader->timeLoc,
        &time,
        SHADER_UNIFORM_FLOAT
    );

    SetShaderValue(
        shader->shader,
        shader->hitTimerLoc,
        &hitTimer,
        SHADER_UNIFORM_FLOAT
    );
}

void UnloadGameShader(GameShader *shader) {
    UnloadShader(shader->shader);
}

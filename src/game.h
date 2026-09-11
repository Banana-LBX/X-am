#ifndef GAME_H
#define GAME_H

#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#include <stdbool.h>

#define WIDTH 1900
#define HEIGHT 1000

// Player
#define PLAYER_SPEED 18
#define PLAYER_RADIUS 16
#define INVINCI_TIME 0.5f

// Bullets
#define BULLET_RADIUS 5
#define BULLET_GAP 20

// Enemies
#define ENEMY_THICKNESS 5.0f

#define LINEAR_HEALTH 100
#define LINEARS_SPEED 4
#define LINEAR_SIZE 35.0f
#define LINEAR_COLOR RED

#define SINE_HEALTH 150
#define SINE_SPEED 3
#define SINE_SIZE 30.0f
#define SINE_COLOR PURPLE
#define SINE_EYE_RADIUS 8
#define SINE_EYE_SPEED 12
#define SINE_EYE_MAX_DIST 2.5

// Attacks
#define ATTACK_THICKNESS 5.0f

#define LINEAR_DAMAGE 10
#define LINEAR_COOLDOWN 1.0f
#define LINEAR_ACTIVE 0.4f

#define SINE_DAMAGE 10
#define SINE_COOLDOWN 0.3f
#define SINE_ACTIVE 0.4f

#define MAX_ATTACKS WIDTH

#endif

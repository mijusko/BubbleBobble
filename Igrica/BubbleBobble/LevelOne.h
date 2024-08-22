#pragma once
#ifndef LEVELONE_H
#define LEVELONE_H

#include "Glut.h"
#include <vector>

extern bool isLevelTwo;
extern bool isLevelThree;

typedef enum {
    NORMAL,
    HIT
} MonsterState;

typedef struct {
    float x, y;
    float width, height;
    MonsterState state;
    bool stopped;
    bool isActive;
} Character;

typedef struct {
    float x, y;
    float radius;
    float speed;
    bool direction; // true za desno, false za levo
} Projectile;

extern Character hero;
extern Character monster1;
extern Character monster2;
extern Character monster3;
extern Character monster4;
extern std::vector<Projectile> projectiles;

extern const double MONSTER1_SPEED;
extern const double MONSTER2_SPEED;
extern const double MONSTER3_SPEED;
extern const double MONSTER4_SPEED;

extern const double Xmin, Xmax;


void drawRectangle(float x, float y, float width, float height);
void drawHero();
void drawMonsters();
void drawPlatforms();
void drawLevelOneScene();
void handleLevelOneKeyboard(unsigned char key, int x, int y);
void updateHero();
bool isCollidingWithPlatform(Character* character, float newX, float newY);
void createProjectile();
void updateProjectiles();
void drawProjectiles();
void updateMonsters();
void checkProjectileCollisions();
void checkHeroMonsterCollisions();
void resetLevel();
void nextLevel();

#endif // LEVELONE_H

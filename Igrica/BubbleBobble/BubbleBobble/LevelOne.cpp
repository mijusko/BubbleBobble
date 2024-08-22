#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../Glut.h"
#include "../LevelOne.h"
#include "../StartScreen.h"

Character hero = { 0.5, 0.3, 0.15, 0.15 };
Character monster1 = { 1.3, 2.2, 0.15, 0.15, NORMAL, false, true };
Character monster2 = { 1.45, 2.2, 0.15, 0.15, NORMAL, false, true };
Character monster3 = { 1.6, 2.2, 0.15, 0.15, NORMAL, false, false };
Character monster4 = { 1.75, 2.2, 0.15, 0.15, NORMAL, false, false };
//                 { X, Y, a, b, pogodjeno?, zaustavljeno?, aktivno?}

const double HERO_STEP = 0.1; // Korak kretanja heroja
const double JUMP_HEIGHT = 0.3; // Visina skoka
const double GRAVITY = 0.01; // Gravitacija
const double JUMP_VELOCITY = 0.12; // Brzina skoka
const double PROJECTILE_SPEED = 0.03; // Brzina projektila
const double PROJECTILE_DISTANCE = 1.0; // Udaljenost koju projektil prelazi

// Brzina svakog cudovista u odnosu na heroja
const double MONSTER1_SPEED = HERO_STEP / 10;
const double MONSTER2_SPEED = HERO_STEP / 16;
const double MONSTER3_SPEED = HERO_STEP / 8;
const double MONSTER4_SPEED = HERO_STEP / 5;

extern const double Xmin, Xmax; // Dodajemo extern za Xmin i Xmax

bool isJumping = false;
double jumpSpeed = 0;
bool lastDirectionRight = true;

int score = 0;
int lives = 3;

std::vector<Projectile> projectiles; // Lista projektila

void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Funkcija za detekciju kolizija sa platformama
bool isCollidingWithPlatform(Character* character, float newX, float newY) {
    // Definisanje platformi
    if (isLevelThree) {
        float platforms[][4] = {
            {0.0, 0.1, 3.0, 0.1},
            {1.1, 0.6, 0.8, 0.1},
            {0.6, 1.1, 0.4, 0.1},
            {1.3, 1.1, 0.4, 0.1},
            {2.0, 1.1, 0.4, 0.1},
            {0.3, 1.6, 0.3, 0.1},
            {1.0, 1.6, 0.3, 0.1},
            {1.7, 1.6, 0.3, 0.1},
            {2.4, 1.6, 0.3, 0.1},
            {1.2, 2.1, 0.6, 0.1}
        };

        for (int i = 0; i < 10; i++) {
            float px = platforms[i][0];
            float py = platforms[i][1];
            float pw = platforms[i][2];
            float ph = platforms[i][3];

            if (newX < px + pw && newX + character->width > px &&
                newY < py + ph && newY + character->height > py) {
                return true;
            }
        }
        return false;
    }
    else if (isLevelTwo) {
        float platforms[][4] = {
            {0.0, 0.1, 3.0, 0.1},
            {0.4, 0.6, 0.4, 0.1},
            {1.25, 0.6, 0.4, 0.1},
            {2.1, 0.6, 0.4, 0.1},
            {0.7, 1.1, 1.6, 0.1},
            {1.0, 1.6, 0.35, 0.1},
            {1.65, 1.6, 0.35, 0.1},
            {1.3, 2.1, 0.4, 0.1}
        };

        for (int i = 0; i < 8; i++) {
            float px = platforms[i][0];
            float py = platforms[i][1];
            float pw = platforms[i][2];
            float ph = platforms[i][3];

            if (newX < px + pw && newX + character->width > px &&
                newY < py + ph && newY + character->height > py) {
                return true;
            }
        }
        return false;
    }
    else {
        float platforms[][4] = {
            {0.0, 0.1, 3.0, 0.1},
            {0, 0.6, 0.3, 0.1},
            {0.7, 0.6, 1.6, 0.1},
            {2.7, 0.6, 0.3, 0.1},
            {0, 1.1, 0.3, 0.1},
            {0.7, 1.1, 1.6, 0.1},
            {2.7, 1.1, 0.3, 0.1},
            {0, 1.6, 0.3, 0.1},
            {0.7, 1.6, 1.6, 0.1},
            {2.7, 1.6, 0.3, 0.1}
        };

        for (int i = 0; i < 10; i++) {
            float px = platforms[i][0];
            float py = platforms[i][1];
            float pw = platforms[i][2];
            float ph = platforms[i][3];

            if (newX < px + pw && newX + character->width > px &&
                newY < py + ph && newY + character->height > py) {
                return true;
            }
        }
        return false;
    }
}


void handleLevelOneKeyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'a': {
        float newX = hero.x - HERO_STEP;
        if (newX >= Xmin && !isCollidingWithPlatform(&hero, newX, hero.y)) {
            hero.x = newX;
            lastDirectionRight = false;
        }
    } break;

    case 'd': {
        float newX = hero.x + HERO_STEP;
        if (newX + hero.width <= Xmax && !isCollidingWithPlatform(&hero, newX, hero.y)) {
            hero.x = newX;
            lastDirectionRight = true;
        }
    } break;

    case 'w':
        if (!isJumping) {
            isJumping = true;
            jumpSpeed = JUMP_VELOCITY;
        }
        break;

    case 'k':
        createProjectile();
        break;
    }
    glutPostRedisplay();
}

// Funkcija za crtanje pravougaonika (kvadrata)
void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Funkcija za crtanje kruga (projektil)
void drawCircle(float x, float y, float radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * 3.14159f * i / 20;
        float dx = cosf(angle) * radius;
        float dy = sinf(angle) * radius;
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}

// Funkcija za crtanje heroja
void drawHero() {
    glColor3f(0.0, 1.0, 0.0); // Zelena boja za heroja
    drawRectangle(hero.x, hero.y, hero.width, hero.height);
}

// Funkcija za crtanje čudovišta
void drawMonsters() {
    for (const auto& monster : { monster1, monster2 ,monster3, monster4 }) {
        if (!monster.isActive) continue; // Preskoči neaktivna čudovišta

        if (monster.state == HIT) {
            glColor3f(1.0, 1.0, 0.0); // Žuta boja za pogođeno čudovište
            drawCircle(monster.x + monster.width / 2, monster.y + monster.height / 2, monster.width / 2);
        }
        else {
            glColor3f(1.0, 0.0, 0.0); // Crvena boja za normalno čudovište
            drawRectangle(monster.x, monster.y, monster.width, monster.height);
        }
    }
}


// Funkcija za crtanje platformi
void drawPlatforms() {
    if(isLevelThree){
        glColor3f(0.0, 0.0, 1.0);

        drawRectangle(0.0, 0.1, 3.0, 0.1);

        drawRectangle(1.1, 0.6, 0.8, 0.1);

        drawRectangle(0.6, 1.1, 0.4, 0.1);
        drawRectangle(1.3, 1.1, 0.4, 0.1);
        drawRectangle(2.0, 1.1, 0.4, 0.1);

        drawRectangle(0.3, 1.6, 0.3, 0.1);
        drawRectangle(1.0, 1.6, 0.3, 0.1);
        drawRectangle(1.7, 1.6, 0.3, 0.1);
        drawRectangle(2.4, 1.6, 0.3, 0.1);


        drawRectangle(1.2, 2.1, 0.6, 0.1);

    }
    else if (isLevelTwo) {
        glColor3f(1.0, 0.5, 0.1);

        drawRectangle(0.0, 0.1, 3.0, 0.1);

        drawRectangle(0.4, 0.6, 0.4, 0.1);
        drawRectangle(1.25, 0.6, 0.4, 0.1);
        drawRectangle(2.1, 0.6, 0.4, 0.1);

        drawRectangle(0.7, 1.1, 1.6, 0.1);

        drawRectangle(1.0, 1.6, 0.35, 0.1);
        drawRectangle(1.65, 1.6, 0.35, 0.1);


        drawRectangle(1.3, 2.1, 0.4, 0.1);

    }
    else{
        glColor3f(0.7, 0.1, 0.7);

        drawRectangle(0.0, 0.1, 3.0, 0.1);

        drawRectangle(0, 0.6, 0.3, 0.1);
        drawRectangle(0.7, 0.6, 1.6, 0.1);
        drawRectangle(2.7, 0.6, 0.3, 0.1);

        drawRectangle(0, 1.1, 0.3, 0.1);
        drawRectangle(0.7, 1.1, 1.6, 0.1);
        drawRectangle(2.7, 1.1, 0.3, 0.1);

        drawRectangle(0, 1.6, 0.3, 0.1);
        drawRectangle(0.7, 1.6, 1.6, 0.1);
        drawRectangle(2.7, 1.6, 0.3, 0.1);
    }
}

// Iscrtavanje trenutnog nivoa
void drawLevelOneScene() {
    if (isLevelThree) {
        glClearColor(0.0, 0.0, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawPlatforms();
        drawHero();
        drawMonsters();
        drawProjectiles();

        glColor3f(1.0, 1.0, 1.0);
        renderBitmapString(0.1f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, "LEVEL: 3");

        char scoreText[20];
        sprintf_s(scoreText, "SCORE: %d", score);
        renderBitmapString(1.4f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, scoreText);

        char livesText[20];
        sprintf_s(livesText, "LIVES: %d", lives);
        renderBitmapString(2.5f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, livesText);

        glutSwapBuffers();
    }
    else if (isLevelTwo) {
        glClearColor(0.2, 0.1, 0.02, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawPlatforms();
        drawHero();
        drawMonsters();
        drawProjectiles();

        glColor3f(1.0, 1.0, 1.0);
        renderBitmapString(0.1f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, "LEVEL: 2");

        char scoreText[20];
        sprintf_s(scoreText, "SCORE: %d", score);
        renderBitmapString(1.4f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, scoreText);

        char livesText[20];
        sprintf_s(livesText, "LIVES: %d", lives);
        renderBitmapString(2.5f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, livesText);

        glutSwapBuffers();
    }
    else {
        glClearColor(0.1, 0.01, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        drawPlatforms();
        drawHero();
        drawMonsters();
        drawProjectiles();

        glColor3f(1.0, 1.0, 1.0);
        renderBitmapString(0.1f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, "LEVEL: 1");

        char scoreText[20];
        sprintf_s(scoreText, "SCORE: %d", score);
        renderBitmapString(1.4f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, scoreText);

        char livesText[20];
        sprintf_s(livesText, "LIVES: %d", lives);
        renderBitmapString(2.5f, 2.9f, GLUT_BITMAP_TIMES_ROMAN_24, livesText);

        glutSwapBuffers();
    }
}


void updateHero() {
    if (isJumping) {
        float newY = hero.y + jumpSpeed;
        if (!isCollidingWithPlatform(&hero, hero.x, newY)) {
            hero.y = newY;
            jumpSpeed -= GRAVITY;
        }
        else {
            isJumping = false;
            jumpSpeed = 0;
        }
    }
    else {
        float newY = hero.y - GRAVITY;
        if (!isCollidingWithPlatform(&hero, hero.x, newY)) {
            hero.y = newY;
        }
        /*else {
            hero.y = round(hero.y * 10.0) / 10.0; // Snapping to the platform height
        }*/
    }
}

// Kretanje cudovista tako da prati poziciju heroja na osnovu koordinata
void updateMonsters() {
    // Ažuriranje pozicije za monster1
    if (!monster1.stopped) {
        if (monster1.x < hero.x) {
            float newX = monster1.x + MONSTER1_SPEED;
            if (newX + monster1.width <= Xmax && !isCollidingWithPlatform(&monster1, newX, monster1.y)) {
                monster1.x = newX;
            }
        }
        else if (monster1.x > hero.x) {
            float newX = monster1.x - MONSTER1_SPEED;
            if (newX >= Xmin && !isCollidingWithPlatform(&monster1, newX, monster1.y)) {
                monster1.x = newX;
            }
        }

        if (monster1.y < hero.y) {
            float newY = monster1.y + MONSTER1_SPEED;
            if (!isCollidingWithPlatform(&monster1, monster1.x, newY)) {
                monster1.y = newY;
            }
        }
        else if (monster1.y > hero.y) {
            float newY = monster1.y - MONSTER1_SPEED;
            if (!isCollidingWithPlatform(&monster1, monster1.x, newY)) {
                monster1.y = newY;
            }
        }
    }

    // Ažuriranje pozicije za monster2
    if (!monster2.stopped) {
        if (monster2.x < hero.x) {
            float newX = monster2.x + MONSTER2_SPEED;
            if (newX + monster2.width <= Xmax && !isCollidingWithPlatform(&monster2, newX, monster2.y)) {
                monster2.x = newX;
            }
        }
        else if (monster2.x > hero.x) {
            float newX = monster2.x - MONSTER2_SPEED;
            if (newX >= Xmin && !isCollidingWithPlatform(&monster2, newX, monster2.y)) {
                monster2.x = newX;
            }
        }

        if (monster2.y < hero.y) {
            float newY = monster2.y + MONSTER2_SPEED;
            if (!isCollidingWithPlatform(&monster2, monster2.x, newY)) {
                monster2.y = newY;
            }
        }
        else if (monster2.y > hero.y) {
            float newY = monster2.y - MONSTER2_SPEED;
            if (!isCollidingWithPlatform(&monster2, monster2.x, newY)) {
                monster2.y = newY;
            }
        }
    }
    // Ažuriranje pozicije za monster3
    if (!monster3.stopped) {
        if (monster3.x < hero.x) {
            float newX = monster3.x + MONSTER3_SPEED;
            if (newX + monster3.width <= Xmax && !isCollidingWithPlatform(&monster3, newX, monster3.y)) {
                monster3.x = newX;
            }
        }
        else if (monster3.x > hero.x) {
            float newX = monster3.x - MONSTER3_SPEED;
            if (newX >= Xmin && !isCollidingWithPlatform(&monster3, newX, monster3.y)) {
                monster3.x = newX;
            }
        }

        if (monster3.y < hero.y) {
            float newY = monster3.y + MONSTER3_SPEED;
            if (!isCollidingWithPlatform(&monster3, monster3.x, newY)) {
                monster3.y = newY;
            }
        }
        else if (monster3.y > hero.y) {
            float newY = monster3.y - MONSTER3_SPEED;
            if (!isCollidingWithPlatform(&monster3, monster3.x, newY)) {
                monster3.y = newY;
            }
        }
    }

    // Ažuriranje pozicije za monster4
    if (!monster4.stopped) {
        if (monster4.x < hero.x) {
            float newX = monster4.x + MONSTER4_SPEED;
            if (newX + monster4.width <= Xmax && !isCollidingWithPlatform(&monster4, newX, monster4.y)) {
                monster4.x = newX;
            }
        }
        else if (monster4.x > hero.x) {
            float newX = monster4.x - MONSTER4_SPEED;
            if (newX >= Xmin && !isCollidingWithPlatform(&monster4, newX, monster4.y)) {
                monster4.x = newX;
            }
        }

        if (monster4.y < hero.y) {
            float newY = monster4.y + MONSTER4_SPEED;
            if (!isCollidingWithPlatform(&monster4, monster4.x, newY)) {
                monster4.y = newY;
            }
        }
        else if (monster4.y > hero.y) {
            float newY = monster4.y - MONSTER4_SPEED;
            if (!isCollidingWithPlatform(&monster4, monster4.x, newY)) {
                monster4.y = newY;
            }
        }
    }

}


void createProjectile() {
    Projectile projectile;
    projectile.x = hero.x + hero.width / 2;
    projectile.y = hero.y + hero.height / 2;
    projectile.radius = 0.08;
    projectile.speed = PROJECTILE_SPEED;
    projectile.direction = lastDirectionRight;
    projectiles.push_back(projectile);
}

void updateProjectiles() {
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        if (it->direction) {
            it->x += it->speed;
        }
        else {
            it->x -= it->speed;
        }

        if (abs(it->x - (hero.x + hero.width / 2)) > PROJECTILE_DISTANCE) {
            it = projectiles.erase(it);
        }
        else {
            ++it;
        }
    }
}

void drawProjectiles() {
    glColor3f(0.0, 1.0, 1.0);
    for (const auto& projectile : projectiles) {
        drawCircle(projectile.x, projectile.y, projectile.radius);
    }
}

// Proveravanje da li je cudoviste pogodjeno
bool isCollisionWithMonster(const Projectile& projectile, const Character& monster) {
        return projectile.x - projectile.radius < monster.x + monster.width &&
            projectile.x + projectile.radius > monster.x &&
            projectile.y - projectile.radius < monster.y + monster.height &&
            projectile.y + projectile.radius > monster.y;
    
}

// Provera smrti cudovista i upravljanje njihovi mstanje i dodavanje poena
void checkProjectileCollisions() {
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        bool hit = false;

        if (isCollisionWithMonster(*it, monster1) && monster1.state == NORMAL && monster1.isActive == true) {
            monster1.state = HIT;
            monster1.stopped = true;
            hit = true;
            score += 1000;
            printf("SCORE: %d", score);
        }
        else if (isCollisionWithMonster(*it, monster2) && monster2.state == NORMAL && monster2.isActive == true) {
            monster2.state = HIT;
            monster2.stopped = true;
            hit = true;
            score += 1000;
            printf("SCORE: %d", score);
        }
        else if (isCollisionWithMonster(*it, monster3) && monster3.state == NORMAL && monster3.isActive == true) {
            monster3.state = HIT;
            monster3.stopped = true;
            hit = true;
            score += 1000;
            printf("SCORE: %d", score);
        }
        else if (isCollisionWithMonster(*it, monster4) && monster4.state == NORMAL && monster4.isActive == true) {
            monster4.state = HIT;
            monster4.stopped = true;
            hit = true;
            score += 1000;
            printf("SCORE: %d", score);
        }

        if (hit) {
            it = projectiles.erase(it); // Ukloni projektil
        }
        else {
            ++it;
        }
    }
}

void resetLevel() {
    hero.x = 0.5;
    hero.y = 0.3;
    monster1.x = 1.3;
    monster1.y = 2.2;
    monster2.x = 1.45;
    monster2.y = 2.2;
    monster3.x = 1.6;
    monster3.y = 2.2;
    monster4.x = 1.75;
    monster4.y = 2.2;

    // Resetuj stanje čudovišta
    monster1.state = NORMAL;
    monster2.state = NORMAL;
    monster3.state = NORMAL;
    monster4.state = NORMAL;
    monster1.stopped = false;
    monster2.stopped = false;
    monster3.stopped = false;
    monster4.stopped = false;

    // Očisti sve projekte
    projectiles.clear();
}


// Upravljanje zivotima heroja
void checkHeroMonsterCollisions() {
    Character* monsters[] = { &monster1, &monster2, &monster3, &monster4 };

    for (auto monster : monsters) {
        if (hero.x + hero.width > monster->x && hero.x < monster->x + monster->width &&
            hero.y + hero.height > monster->y && hero.y < monster->y + monster->height) {
            if (monster->state == HIT && monster->isActive == true) {
                score += 500;
                monster->isActive = false;
            }
            else {
                if (monster->isActive == true) {
                    lives--;
                    resetLevel();
                }
                if (lives <= 0) {
                    printf("Game Over!\n");                 
                    isGameStarted = false;
                    score = 0;
                    lives = 3;
                    
                }
            }
        }
    }
}

// Proveravanje trenutnog nivoa i prelazak u sledeci
bool isLevelTwo = false;
bool isLevelThree = false;

void nextLevel() {
    if (monster1.isActive == false && monster2.isActive == false && isGameStarted && isLevelTwo == false && isLevelThree == false) {
        isLevelTwo = true;
        resetLevel();
        monster1.isActive = true;
        monster2.isActive = true;
        monster3.isActive = true;
    }
    else if (monster1.isActive == false && monster2.isActive == false && monster3.isActive == false && isLevelTwo == true) {
        isLevelTwo = false;
        isLevelThree = true;
        resetLevel();
        monster1.isActive = true;
        monster2.isActive = true;
        monster3.isActive = true;
        monster4.isActive = true;
    }
}
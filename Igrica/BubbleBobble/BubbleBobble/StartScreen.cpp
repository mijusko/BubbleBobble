#include <math.h>		
#include <stdio.h>
#include <stdlib.h>		
#include <string.h>
#include "../Glut.h" 
#include "../LevelOne.h"

// Funkcija za crtanje pravougaonika
void drawRectangle(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Funkcija za prikazivanje teksta u određenoj boji
void drawText(const char* text, float x, float y, float r, float g, float b, void* font) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(font, text[i]);
    }
}

// Funkcija za crtanje početne stranice
void drawStartScreen() {
    glClearColor(0.0, 0.2, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // Prikaz naslova igre (žuto) - Veći tekst
    drawText("Bubble Bobble", 1.2, 2.5, 1.0, 1.0, 0.0, GLUT_BITMAP_TIMES_ROMAN_24);

    // Prikaz dugmeta Start Game
    drawRectangle(1.0, 1.5, 1.0, 0.4, 1.0, 1.0, 0.0);
    drawText("Start Game", 1.3, 1.65, 0.0, 0.0, 1.0, GLUT_BITMAP_TIMES_ROMAN_24);

    // Prikaz dugmeta Exit
    drawRectangle(1.0, 1.0, 1.0, 0.4, 1.0, 1.0, 0.0);
    drawText("Exit", 1.4, 1.15, 0.0, 0.0, 1.0, GLUT_BITMAP_TIMES_ROMAN_24);

    glutSwapBuffers();
}

bool isGameStarted = false;

// Funkcija za rukovanje klikovima miša
void handleMouseClick(int button, int state, int x, int y) {
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    float normalizedX = (float)x / windowWidth * 3.0f;
    float normalizedY = (float)(windowHeight - y) / windowHeight * 3.0f;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Koordinate za dugme Start Game
        if (normalizedX >= 1.0f && normalizedX <= 2.0f && normalizedY >= 1.5f && normalizedY <= 1.9f) {
            isGameStarted = true;
            isLevelTwo = false;
            isLevelThree = false;
            resetLevel();
            monster3.isActive = false;
            monster4.isActive = false;
        }
        // Koordinate za dugme Exit
        else if (normalizedX >= 1.0f && normalizedX <= 2.0f && normalizedY >= 1.0f && normalizedY <= 1.4f) {
            exit(0);
        }
    }
}

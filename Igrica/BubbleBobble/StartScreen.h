#pragma once
#ifndef STARTSCREEN_H
#define STARTSCREEN_H

extern bool isGameStarted;

void drawText(const char* text, float x, float y);
void drawStartScreen();
void handleMouseClick(int button, int state, int x, int y);

#endif // STARTSCREEN_H

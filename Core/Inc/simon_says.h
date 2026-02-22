#ifndef __SIMON_SAYS_H
#define __SIMON_SAYS_H

#include "main.h"
#include <stdlib.h>
#include <stdio.h>

#define BOX_SIZE    80
#define GAP         15
#define START_X     150
#define START_Y     90

extern TS_State_t TS_State;
extern uint8_t sequence[30];
extern uint8_t seqLen;

void Simon_Init(void);
void simonUI(int level, int highScore);
uint8_t getTouchPosition(uint16_t x, uint16_t y);
void buttonFlash(uint8_t button, uint32_t duration);
void addStep(void);
void showSequence(void);
void gameOver(uint8_t pressed, uint8_t correct, int highScore, int newHS);

#endif

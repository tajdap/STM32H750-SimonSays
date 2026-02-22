/*
 * simon_says.c
 *
 *  Created on: 30 Jan 2026
 *      Author: Tajda Pristavec
 */

#include "main.h"
#include "simon_says.h"

TS_State_t TS_State;
TS_Init_t hTS;

uint8_t sequence[30];
uint8_t seqLen = 0;

//Initialize Touch
void Simon_Init(void) {
	uint32_t x_size, y_size;
	BSP_LCD_GetXSize(0, &x_size);
	BSP_LCD_GetYSize(0, &y_size);

	hTS.Width = x_size;
	hTS.Height = y_size;
	hTS.Orientation = TS_SWAP_XY;
	hTS.Accuracy = 5;

	BSP_TS_Init(0, &hTS); // Initialize sensor
}

//UI Display
void simonUI(int level, int highScore) {
	UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

	//Display text
	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
	UTIL_LCD_SetFont(&Font24);
	UTIL_LCD_DisplayStringAt(0, 10, (uint8_t *)"SIMON SAYS", CENTER_MODE);

	char level_str[20] = "";
	UTIL_LCD_SetFont(&Font12);

	sprintf(level_str, "level: %d", level);
	UTIL_LCD_DisplayStringAt(0, 45, (uint8_t *)level_str, CENTER_MODE);

	sprintf(level_str, "high score: %d", highScore);
	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
	UTIL_LCD_DisplayStringAt(0, 60, (uint8_t *)level_str, CENTER_MODE);

	//Draw squares
	UTIL_LCD_DrawRect(START_X, START_Y, BOX_SIZE, BOX_SIZE, UTIL_LCD_COLOR_GREEN);
	UTIL_LCD_DrawRect(START_X + BOX_SIZE + GAP, START_Y, BOX_SIZE, BOX_SIZE, UTIL_LCD_COLOR_RED);
	UTIL_LCD_DrawRect(START_X, START_Y + BOX_SIZE + GAP, BOX_SIZE, BOX_SIZE, UTIL_LCD_COLOR_YELLOW);
	UTIL_LCD_DrawRect(START_X + BOX_SIZE + GAP, START_Y + BOX_SIZE + GAP, BOX_SIZE, BOX_SIZE, UTIL_LCD_COLOR_BLUE);
}

//Touch sensor
uint8_t getTouchPosition(uint16_t x, uint16_t y) {
	//Green
	if ((x >= START_X) && (x <= START_X + BOX_SIZE) &&
		(y >= START_Y) && (y <= START_Y + BOX_SIZE)) {
		return 1;
	}
	//Red
	if ((x >= START_X + BOX_SIZE + GAP) && (x <= START_X + 2*BOX_SIZE + GAP) &&
		(y >= START_Y) && (y <= START_Y + BOX_SIZE)) {
		return 2;
	}
	//Yellow
	if ((x >= START_X) && (x <= START_X + BOX_SIZE) &&
		(y >= START_Y + BOX_SIZE + GAP) && (y <= START_Y + 2*BOX_SIZE + GAP)) {
		return 3;
	}
	//Blue
	if ((x >= START_X + BOX_SIZE + GAP) && (x <= START_X + 2*BOX_SIZE + GAP) &&
		(y >= START_Y + BOX_SIZE + GAP) && (y <= START_Y + 2*BOX_SIZE + GAP)) {
		return 4;
	}
	return 0;
}

//Flashing buttons
void buttonFlash(uint8_t button, uint32_t duration) {
	uint16_t x, y;
	uint32_t color;

	if (button == 1) {
		x = START_X;
		y = START_Y;
		color = UTIL_LCD_COLOR_GREEN;
	} else if (button == 2) {
		x = START_X + BOX_SIZE + GAP;
		y = START_Y;
		color = UTIL_LCD_COLOR_RED;
	} else if (button == 3) {
		x = START_X;
		y = START_Y + BOX_SIZE + GAP;
		color = UTIL_LCD_COLOR_YELLOW;
	} else if (button == 4) {
		x = START_X + BOX_SIZE + GAP;
		y = START_Y + BOX_SIZE + GAP;
		color = UTIL_LCD_COLOR_BLUE;
	} else {
		return;
	}

	UTIL_LCD_FillRect(x, y, BOX_SIZE, BOX_SIZE, color);
	HAL_Delay(duration);

	UTIL_LCD_FillRect(x, y, BOX_SIZE, BOX_SIZE, UTIL_LCD_COLOR_BLACK);
	UTIL_LCD_DrawRect(x, y, BOX_SIZE, BOX_SIZE, color);
}

//Add a step to the sequence
void addStep(void) {
	if (seqLen < 30) {
		sequence[seqLen] = (rand() % 4) + 1;
		seqLen++;
	}
}

void showSequence(void) {
	for (int i = 0; i < seqLen; i++) {
		buttonFlash(sequence[i], 500);
		HAL_Delay(200);
	}
}

void gameOver(uint8_t pressed, uint8_t correct, int highScore, int newHS) {
	BSP_LED_On(LED_RED);

	for (int i = 0; i < 3; i++) {
		buttonFlash(pressed, 100);
		HAL_Delay(100);
	}
	buttonFlash(correct, 1000);

	UTIL_LCD_Clear(UTIL_LCD_COLOR_BLACK);

	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_RED);
	UTIL_LCD_SetBackColor(UTIL_LCD_COLOR_BLACK);
	UTIL_LCD_SetFont(&Font24);
	UTIL_LCD_DisplayStringAt(0, 120, (uint8_t *)"  GAME OVER  ", CENTER_MODE);

	char curLevel[30];
	sprintf(curLevel, "Score: %d", highScore);
	UTIL_LCD_SetFont(&Font12);
	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
	UTIL_LCD_DisplayStringAt(0, 150, (uint8_t *)curLevel, CENTER_MODE);

	if (newHS) {
		UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_YELLOW);
		UTIL_LCD_SetFont(&Font16);
	    UTIL_LCD_DisplayStringAt(0, 180, (uint8_t *)"NEW HIGH SCORE!", CENTER_MODE);
	}

	UTIL_LCD_SetFont(&Font12);
	UTIL_LCD_SetTextColor(UTIL_LCD_COLOR_WHITE);
	UTIL_LCD_DisplayStringAt(0, 240, (uint8_t *)"TOUCH TO RESTART", CENTER_MODE);
}

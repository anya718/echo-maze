#pragma once

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // TODO: separate .c and .h files
#define SCREEN_HEIGHT 32

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define DEBOUNCE    150
#define NEXT_LVL    300

void display_init();
void display_text(char *text);
void display_movement(int player_row, int player_col);
void display_wall(int player_row, int player_col);
void display_start();

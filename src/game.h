#pragma once

// #include <stdint.h>
#include "display.h"
#include "utils.h"

#define DIF_NUM 2
#define MAX_ROWS 11

bool is_finished();
int tone_freq();
bool wall_up();
bool wall_down();
bool wall_left();
bool wall_right();
void choose_difficulty();
void finish();
void start();
void play_level();

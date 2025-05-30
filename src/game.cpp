#include "game.h"



const int rows[DIF_NUM] = {10};
const int cols[DIF_NUM] = {9};
const int finish_row[DIF_NUM] = {9};
const int finish_col[DIF_NUM] = {3};
const int start_row[DIF_NUM] = {1};
const int start_col[DIF_NUM] = {3};

const char* difficulties[DIF_NUM] = {
    "      Very Easy", 
    "         Easy", 
    "        Medium", 
    "         Hard", 
    "      Very Hard", 
    "       Extreme"
};

const char* diff_maze[DIF_NUM] = {
    "    very easy maze", 
    "      easy maze", 
    "     medium maze", 
    "      hard maze", 
    "    very hard maze", 
    "     extreme maze"
};

int player_row;
int player_col;
int dist_range = 11;
int difficulty = 0;

int8_t maze[10][9] = {
    {-1, -1, -1, -1, -1, -1, -1, -1, -1},
    {-1,  5,  0,  6,  0,  7,  0,  8, -1},
    {-1,  0, -1, -1, -1, -1, -1,  0, -1},
    {-1,  4, -1, 11,  0, 10,  0,  9, -1},
    {-1,  0, -1, -1, -1, -1, -1, -1, -1},
    {-1,  3,  0,  2, -1,  3,  0,  4, -1},
    {-1,  0, -1,  0, -1,  0, -1,  0, -1},
    {-1,  4, -1,  1,  0,  2, -1,  5, -1},
    {-1, -1, -1,  0, -1, -1, -1, -1, -1}
};

bool is_finished() {
    return player_row == finish_row[difficulty] && player_col == finish_col[difficulty];
}

int tone_freq() {
    return 100 + (dist_range - maze[player_row][player_col]) * (900 / (dist_range - 1));
}

bool wall_up() {
    return maze[player_row - 1][player_col] == -1;
}

bool wall_down() {
    return maze[player_row + 1][player_col] == -1;
}

bool wall_left() {
    return maze[player_row][player_col - 1] == -1;
}

bool wall_right() {
    return maze[player_row][player_col + 1] == -1;
}

void choose_difficulty() {
    while (1) {
        char text[100];
        sprintf(text, "  Choose difficulty:\n\n%s", difficulties[difficulty]);
        display_text(text);

        if (button_pressed(BTN_L)) {
            if (difficulty)
                difficulty--;
            _delay_ms(DEBOUNCE);
        }

        if (button_pressed(BTN_R)) {
            if (difficulty != 5)
                difficulty++;
            _delay_ms(DEBOUNCE);
        }
        
        if (button_pressed(BTN_U) || button_pressed(BTN_D))
            break;
    }
    char text[100];
    sprintf(text, "   Good luck, hero!\n\n    You enter the\n%s\n", diff_maze[difficulty]);
    display_text(text);
    _delay_ms(200);
}

void finish() {
    led_on(LED_BLTIN);
    display_text("   Well done, hero!\n   You've conquered\n      the maze!\n   Press any button");
    noTone(BUZZ); 
    _delay_ms(NEXT_LVL);
}

void start() {
    display_start();
    choose_difficulty();
}

void play_level() {
    player_row = start_row[difficulty];
    player_col = start_col[difficulty];

    while(1) {
        if (is_finished()) {
            if (any_button_pressed()) {
                break;
            } else {
                continue;
            }
        }

        if (button_pressed(BTN_U)) {
            if (wall_up()) {
                tone (BUZZ, 50);
                display_wall(player_row, player_col);
                continue;
            }

            player_row -= 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement(player_row, player_col);
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);

        } else if (button_pressed(BTN_D)) {
            if (wall_down()) {
                tone (BUZZ, 50);
                display_wall(player_row, player_col);
                continue;
            }

            player_row += 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement(player_row, player_col);
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);

        } else if (button_pressed(BTN_L)) {
            if (wall_left()) {
                tone (BUZZ, 50);
                display_wall(player_row, player_col);
                continue;
            }

            player_col -= 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement(player_row, player_col);
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);

        } else if (button_pressed(BTN_R)) {
            if (wall_right()) {
                tone (BUZZ, 50);
                display_wall(player_row, player_col);
                continue;
            }

            player_col += 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement(player_row, player_col);
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);
        } else {
            led_off(LED_BLTIN);
            noTone(BUZZ);  
        }
    }
}
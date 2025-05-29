#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // TODO: separate .c and .h files
#define SCREEN_HEIGHT 32

#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 oled_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define BUZZ        PD6
#define LED_BLTIN   PB5
#define BTN_R       PB4
#define BTN_U       PB3
#define BTN_L       PB2
#define BTN_D       PB1

#define DEBOUNCE    150
#define NEXT_LVL    300

#define ROWS 10 // TODO: add more mazes and choose difficulty
#define COLS 9
const int finish_row = 9;
const int finish_col = 3;
const int start_row = 1;
const int start_col = 3;
int player_row;
int player_col;
int dist_range = 11;

int difficulty = 0;
const char* difficulties[6] = {
    "      Very easy", 
    "         Easy", 
    "        Medium", 
    "         Hard", 
    "      Very Hard", 
    "       Extreme"
};

int8_t maze[ROWS][COLS] = {
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

static void GPIO_init() {
    /* Setăm pinul PB5(led builtin) ca pin de ieșire. */
	DDRB |= (1 << LED_BLTIN);
    /* Setăm pinii PB1-4 (D9-12) ca pini de intrare (butoane). */
	DDRB &= ~(1 << BTN_U);
	DDRB &= ~(1 << BTN_D);
	DDRB &= ~(1 << BTN_L);
	DDRB &= ~(1 << BTN_R);    

    /* LED pe low */
    PORTB &= ~(1 << LED_BLTIN);
    /* Activati rezistentele de PULL-UP interne microcontroller-ului */
	PORTB |= (1 << BTN_U);
	PORTB |= (1 << BTN_D);
	PORTB |= (1 << BTN_L);
	PORTB |= (1 << BTN_R);

    init();
    pinMode(BUZZ, OUTPUT); // TODO: add buzzer with pwm
}

static void display_init() {
    if(!oled_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

void display_text(char *text) {
    oled_display.clearDisplay();
    oled_display.setTextSize(1);
    oled_display.setTextColor(SSD1306_WHITE);
    oled_display.setCursor(0, 0);
    oled_display.println(text);
    oled_display.display();
    delay(100);
}

bool button_pressed(int button) {
    return bit_is_clear(PINB, button);
}

bool any_button_pressed() {
    return button_pressed(BTN_U) || button_pressed(BTN_D) || button_pressed(BTN_L) || button_pressed(BTN_R);
}

void led_on(int led) {
    PORTB |= (1 << led);
}

void led_off(int led) {
    PORTB &= ~(1 << led);
}

bool is_finished() {
    return player_row == finish_row && player_col == finish_col;
}

int tone_freq() {
    return 100 + (dist_range - maze[player_row][player_col]) * (900 / (dist_range - 1));
}

void display_movement() {
    char text[100];
    sprintf(text, "MOVED!\n %d %d", player_row, player_col);
    display_text(text);
}

void display_wall() {
    char text[100];
    sprintf(text, "WALL!\n %d %d", player_row, player_col);
    display_text(text);
}

bool wall_up() {
    return maze[player_row - 1][player_col] == -1; // TODO: make movement file
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

void display_start() {
    display_text("     Start game!\n\n   <move to begin>");
    _delay_ms(DEBOUNCE);
    while (1) {
        if (any_button_pressed()) {
            _delay_ms(DEBOUNCE);
            break;
        }
    }
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
            _delay_ms(DEBOUNCE); // WHERE?
        }
        
        if (button_pressed(BTN_U) || button_pressed(BTN_D))
            break;
    }
    char text[100];
    sprintf(text, "You enter the maze!\nDifficulty:\n%s", difficulties[difficulty]);
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
    player_row = start_row;
    player_col = start_col;

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
                display_wall();
                continue;
            }

            player_row -= 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement();
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);

        } else if (button_pressed(BTN_D)) {
            if (wall_down()) {
                tone (BUZZ, 50);
                display_wall();
                continue;
            }

            player_row += 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement();
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);

        } else if (button_pressed(BTN_L)) {
            if (wall_left()) {
                tone (BUZZ, 50);
                display_wall();
                continue;
            }

            player_col -= 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement();
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);

        } else if (button_pressed(BTN_R)) {
            if (wall_right()) {
                tone (BUZZ, 50);
                display_wall();
                continue;
            }

            player_col += 2;
            if (is_finished()) {
                finish();
                continue;
            }

            display_movement();
            tone (BUZZ, tone_freq());
            _delay_ms(DEBOUNCE);
        } else {
            led_off(LED_BLTIN);
            noTone(BUZZ);  
        }
    }
}

int main() {
    GPIO_init();
    display_init();

	while(1) {
        start();
        play_level();
	}
 
	return 0;
}

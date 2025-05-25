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
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define BUZZ PD6
#define BTN_U PB4
#define BTN_D PB1
#define BTN_L PB3
#define BTN_R PB2
#define LED_BLTIN PB5

#define ROWS 10 // TODO: add more mazes and choose randomly
#define COLS 9
const int finish_row = 9;
const int finish_col = 3;
int player_row = 1;
int player_col = 3;

char maze[ROWS][COLS] = { // TODO: decide which matrix type is best
    {'0','0','0','0','0','0','0','0','0'},
    {'0','5',' ','6',' ','7',' ','8','0'},
    {'0',' ','0','0','0','0','0',' ','0'},
    {'0','4','0','9',' ','9',' ','9','0'},
    {'0',' ','0','0','0','0','0','0','0'},
    {'0','3',' ','2','0','3',' ','4','0'},
    {'0',' ','0',' ','0',' ','0',' ','0'},
    {'0','4','0','1',' ','2','0','5','0'},
    {'0','0','0',' ','0','0','0','0','0'}
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
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

void display_text(char *text) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(text);
    display.display();
    delay(100);
}

bool button_pressed(int button) {
    return bit_is_clear(PINB, button);
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
    return (10 - (maze[player_row][player_col] - '0')) * 100;
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
    return maze[player_row - 1][player_col] == '0'; // TODO: make movement file
}

bool wall_down() {
    return maze[player_row + 1][player_col] == '0';
}

bool wall_left() {
    return maze[player_row][player_col - 1] == '0';
}

bool wall_right() {
    return maze[player_row][player_col + 1] == '0';
}

void start() {
    display_text("     Start game!\n\n   <move to begin>");
}

void finish() {
    led_on(LED_BLTIN);
    display_text("   Congratulations!\n   You've conquered\n      the maze!\n   Well done, hero!");
    noTone(BUZZ); 
}

int main() {
    GPIO_init();
    display_init();

    start();

	while(1) {
        if (is_finished())
            continue;

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
            if (is_finished()) finish();
            _delay_ms(100);

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
            _delay_ms(100);

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
            _delay_ms(100);

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
            _delay_ms(100);
        } else {
            led_off(LED_BLTIN);
			noTone(BUZZ);  
        }
	}
 
	return 0;
}

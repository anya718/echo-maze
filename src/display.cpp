#include "display.h"
#include "utils.h"

Adafruit_SSD1306 oled_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void display_init() {
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

void display_movement(int player_row, int player_col) {
    oled_display.clearDisplay();
    oled_display.setTextSize(1);
    oled_display.setTextColor(SSD1306_WHITE);
    oled_display.setCursor(0, 0);

    oled_display.println("     MOVED!");
    oled_display.println();

    oled_display.print(player_row);
    oled_display.print(" ");
    oled_display.print(player_col);

    oled_display.display();
    delay(100);
}

void display_wall(int player_row, int player_col) {
    oled_display.clearDisplay();
    oled_display.setTextSize(1);
    oled_display.setTextColor(SSD1306_WHITE);
    oled_display.setCursor(0, 0);

    oled_display.println("       Bonk!");
    oled_display.println();
    oled_display.println("   That's a wall!");
    // oled_display.print(player_row);
    // oled_display.print(" ");
    // oled_display.print(player_col);

    oled_display.display();
    delay(100);
}


void display_start() {
    display_text("Step inside the maze!\n\n   <move to begin>");
    _delay_ms(DEBOUNCE);
    while (1) {
        if (any_button_pressed()) {
            _delay_ms(DEBOUNCE);
            break;
        }
    }
}

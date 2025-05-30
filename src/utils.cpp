#include "utils.h"

void GPIO_init() {
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
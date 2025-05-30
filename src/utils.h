#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#define BUZZ        PD6
#define LED_BLTIN   PB5
#define BTN_R       PB4
#define BTN_U       PB3
#define BTN_L       PB2
#define BTN_D       PB1

void GPIO_init();
bool button_pressed(int button);
bool any_button_pressed();
void led_on(int led);
void led_off(int led);

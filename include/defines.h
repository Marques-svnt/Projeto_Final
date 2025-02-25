#ifndef DEFINES_H
#define DEFINES_H

// Definições relacioanadas ao I2C
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Definir os pinos dos leds RGB e o Botão A e B
#define BUTTON_A 5
#define BUTTON_B 6
#define VERDE 11
#define AZUL 12
#define VERMELHO 13
#define BUZZER 21

// Definições do joystick

#define JOYSTICK_Y_PIN 27

// Definições da matriz de led
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define IS_RGBW false

#endif
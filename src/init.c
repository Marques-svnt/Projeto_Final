#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "display.h"
#include "pwm.h"
#include "pio.h"
#include "defines.h"

// Inicializa o joystick
void joystick_init()
{
    // Configura o ADC do joystick e o estado do botão
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);
}

// Configura o botão A e o led verde
void ledg_button_init(){

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(VERDE);
    gpio_set_dir(VERDE, GPIO_OUT);
}

// Faz todas as inicializações do projeto
void init()
{
    stdio_init_all();
    initI2C();
    joystick_init();
    ledg_button_init();
    pwm_slice_init();
    initializePio();
}
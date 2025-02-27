// Bibliotecas padrão em C
#include <stdio.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Headers do projeto
#include "defines.h"

// Inicializa o PWM
uint pwm_init_gpio(uint gpio, uint wrap)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true);
    return slice_num;
}

// Inicializa o slice e configura para cada led
void pwm_slice_init()
{
    uint pwm_wrap = 4096;
    uint pwm_slice_vermelho = pwm_init_gpio(VERMELHO, pwm_wrap);
    uint pwm_slice_azul = pwm_init_gpio(AZUL, pwm_wrap);
    uint pwm_slice_verde = pwm_init_gpio(VERDE, 2048);
}


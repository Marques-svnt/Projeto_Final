#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "defines.h"

uint pwm_init_gpio(uint gpio, uint wrap)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true);
    return slice_num;
}

void pwm_slice_init()
{
    uint pwm_wrap = 4096;
    uint pwm_slice_vermelho = pwm_init_gpio(VERMELHO, pwm_wrap);
    uint pwm_slice_azul = pwm_init_gpio(AZUL, pwm_wrap);
    uint pwm_slice_verde = pwm_init_gpio(VERDE, 2048);
}


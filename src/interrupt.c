#include <stdio.h>
#include "pico/stdlib.h"
#include "defines.h"
#include "display.h"
#include "pwm.h"

// Variáveis globais
static volatile uint a = 1;
static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento (em microssegundos)

// Função responsável pelo debounce
bool debounce(volatile uint32_t *last_time, uint32_t debounce_time) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > debounce_time) {
        *last_time = current_time;
        return true;
    }
    return false;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());


}

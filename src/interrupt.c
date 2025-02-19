#include <stdio.h>
#include "pico/stdlib.h"
#include "defines.h"
#include "display.h"
#include "pio.h"

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0;  // Intensidade do vermelho
uint8_t led_g = 0;  // Intensidade do verde
uint8_t led_b = 20; // Intensidade do azul

// Variáveis globais
static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time_B = 0;
static volatile uint32_t last_time_J = 0;
static volatile int choose = 0;

extern int state;

void display_menu()
{
    switch (choose)
    {
    case 0:
        limpar();
        display("Sensor", 45, 20);
        display("Temperatura", 25, 35);
        break;
    case 1:
        limpar();
        display("Sensor ", 45, 20);
        display("Movimento", 25, 35);
        break;
    case 2:
        limpar();
        display("Configurar", 30, 20);
        break;
    }
}

// Função responsável pelo debounce
bool debounce(volatile uint32_t *last_time, uint32_t debounce_time)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > debounce_time)
    {
        *last_time = current_time;
        return true;
    }
    return false;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Incrementação do número da matriz de leds
    if (gpio == BUTTON_A && debounce(&last_time_A, 200000))
    {
        last_time_A = current_time;
        if (choose == 0)
        {
            choose = 2;
        }
        else
        {
            choose--;
        }
        set_one_led(choose, led_r, led_g, led_b);
        display_menu(choose);
    }
    // Decrementação do número da matriz de leds
    else if (gpio == BUTTON_B && debounce(&last_time_B, 200000))
    {
        last_time_B = current_time;

        if (choose == 2)
        {
            choose = 0;
        }
        else
        {
            choose++;
        }
        set_one_led(choose, led_r, led_g, led_b);
        display_menu(choose);
    }
    else if (gpio == JOYSTICK_PB && debounce(&last_time_B, 200000))
    {
        last_time_J = current_time;
        state = choose; // Atribui ao estado de maquina o valor atual do menu ao pressionar o joystick
        set_one_led(0, 0, 0, 0); // Limpa a matriz de leds
        limpar(); // Limpa o display
    }
}

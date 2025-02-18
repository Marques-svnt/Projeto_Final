#include <stdio.h>
#include "pico/stdlib.h"
#include "defines.h"
#include "init.h"
#include "interrupt.h"
#include "pwm.h"

volatile int exec = 1;                    /* 1 - Enquanto o código estiver executando
                                             0 - Enquanto quiser sair*/

static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time_B = 0;
static volatile uint32_t last_time_J = 0;

void gpio_irq_handler_temp(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Troca a unidade de medida
    if (gpio == BUTTON_A && debounce(&last_time_A, 200000))
    {
        last_time_A = current_time;
        exec = 0;
        printf("teste");
    }
    // Volta para o menu
    else if (gpio == BUTTON_B && debounce(&last_time_B, 200000))
    {
        last_time_B = current_time;
    }
}

int temperatura()
{
    printf("estou na temp");
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_temp);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_temp);

    while (exec == 1)
    {
        sleep_ms(50);
    }
    menu_init();
    return 0;
}

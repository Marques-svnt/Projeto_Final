// Bibliotecas padrão em C
#include <stdio.h>
#include <stdlib.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"

// Headers do projeto
#include "defines.h"
#include "display.h"
#include "init.h"
#include "menu.h"
#include "pwm.h"
#include "pio.h"


int main()
{

    // Função que chama todas as inicializações do projeto
    init();

    set_one_led(1, 0, 0, 20);

    display("Sensor", 45, 20);
    display("Temperatura", 25, 35);

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    
    // Loop principal
    while (true)
    {
        sleep_ms(50);
    }
}

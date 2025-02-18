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
#include "interrupt.h"
#include "pwm.h"
#include "pio.h"

// Headers dos sensores
#include "temperatura.h"

volatile int state = 0; /* 0 - Menu (inicialmente configurado)
                           1 - Sensor de Temperatura
                           2 - Sensor de Temperatura
                           3 - Sensor de Temperatura*/
int main()
{

    // Função que chama todas as inicializações do projeto
    init();

    // Inicia os parâmetros de botões do menu
    menu_init();

    // Loop principal
    while (true)
    {
        switch (state)
        {
        case 1:
            state = 0; // Limpa o buffer
            temperatura();
            printf("Temp\n");
            break;
        case 2:
            state = 0; // Limpa o buffer
            // Código do alarme
            printf("alarme\n");
            break;
        case 3:
            state = 0; // Limpa o buffer
            // Código da freq
            printf("freq\n");
            break;
        case 4:
            state = 0; // Limpa o buffer
            // Código da config
            printf("Config\n");
            break;
        }
        sleep_ms(50);
    }
}

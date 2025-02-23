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

// Headers do simulador
#include "temperatura.h"
#include "config_uart.h"

volatile int state = 0; /* 0 - Menu (inicialmente configurado)
                           1 - Sensor de Temperatura
                           2 - Configurações*/
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
            config();
            printf("Config\n");
            break;
        }
        sleep_ms(50);
    }
}

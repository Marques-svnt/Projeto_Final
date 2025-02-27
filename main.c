/* Para usar corretamente a configuração via UART, abra o serial monitor e configure a opção Line ending como LF
Para gerar os relatórios como .txt, clique na engrenagem, escolha uma pasta em Choose Log File Directory e depois ative a opção Toggle File Logging*/

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
#include "defines.h"         // Definições globais e configurações do projeto
#include "display.h"         // Funções de controle do display
#include "init.h"            // Funções de inicialização do hardware
#include "interrupt.h"       // Funções de manipulação de interrupções
#include "pwm.h"             // Controle de PWM
#include "pio.h"             // Funções PIO para matriz 5x5

// Headers do simulador
#include "temperatura.h"     // Funções relacionadas ao sensor de temperatura
#include "config_uart.h"     // Funções de configuração e comunicação UART

// Estado atual do menu
volatile int state = 0;  /* 0 - Menu (inicialmente configurado)
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
            temperatura(); // Função para ler a temperatura
            break;
        case 2:
            state = 0; // Limpa o buffer
            config(); // Função de configuração
            break;
        }
        sleep_ms(50); // Atraso para evitar loop excessivo
    }
}

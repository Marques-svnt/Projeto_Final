#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "config_uart.h"
#include "init.h"
#include "pico/bootrom.h"

// Váriaveis globais configuráveis  padrão do projeto
volatile float temp_min = 2.0;
volatile float temp_max = 8.0;
volatile float incremento = 5.0;

int config_temp()
{
}

int config()
{
    if (stdio_usb_connected())
    {
        printf("\033[H\033[J"); // Limpa o serial
        printf("------------------------------------------Configurações-----------------------------------------\n");
        printf("1 - Configurações do sensor de temperatura\n");
        printf("2 - Configurações de relatório\n");
        printf("3 - Restaurar configurações de fábrica\n");
        printf("4 - Entrar em bootsel\n");
        printf("5 - Voltar ao menu\n");
        printf("Escolha uma opção: ");

        char entrada = getchar();      // Captura um único caractere
        int escolha_1 = entrada - '0'; // Converte de char para inteiro

        if (!(escolha_1 >= 1 && escolha_1 <= 5)) // Verifica entrada inválida
        {
            printf("Opção inválida!\n\n");
            return config(); // Chama a função novamente
        }

        switch (escolha_1)
        {
        case 1:
            config_temp();
            break;
        case 2:
            // Código das configurações gerais
            break;
        case 3:
            // Código das configurações gerais
            break;
        case 4:
            display("Entrando em", 20, 22);
            display("BOOTSEL", 36, 32);
            sleep_ms(1000);
            display("           ", 20, 22); // Limpa o display
            display("       ", 36, 32);

            reset_usb_boot(0, 0); // Entra em modo bootsell

            break;
        case 5:
            break;
        }
    }
    else
    {
        limpar();
        display("Ative o Serial", 8, 10);
        display("Monitor", 36, 22);
        display("e tente", 36, 34);
        display("novamente", 28, 46);
        sleep_ms(4000);
    }

    menu_init(); // configura novamente os botões do menu
    return 0;
}

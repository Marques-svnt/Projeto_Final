#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "display.h"
#include "defines.h"
#include "config_uart.h"
#include "init.h"
#include "pico/bootrom.h"

// Valores de fábrica
const float temp_min_fabrica = 2.0;
const float temp_max_fabrica = 8.0;
const float incremento_fabrica = 5.0;

// Váriaveis globais configuráveis do projeto
volatile float temp_min = temp_min_fabrica;
volatile float temp_max = temp_max_fabrica;
volatile float incremento = incremento_fabrica;

// Função para resetar as configurações para os valores de fábrica
void reset_config_fabrica()
{
    temp_min = temp_min_fabrica;
    temp_max = temp_max_fabrica;
    incremento = incremento_fabrica;
    printf("Configurações de fábrica restauradas.\n");
}

// Função para verificar se as configurações estão no valor de fábrica
bool verificar_config_fabrica()
{
    if (temp_min == temp_min_fabrica && temp_max == temp_max_fabrica && incremento == incremento_fabrica)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int config_temp()
{
    printf("\033[H\033[J"); // Limpa o serial
    printf("1 - Alterar valor da temperatura máxima\n");
    printf("2 - Alterar valor da temperatura mínima\n");
    printf("3 - Alterar valor do incremento\n");
    printf("4 - Salvar configurações e sair\n");
    printf("5 - Sair sem salvar\n");

    char entrada = getchar();    // Captura um único caractere
    int escolha = entrada - '0'; // Converte de char para inteiro

    if (!(escolha >= 1 && escolha <= 5)) // Verifica entrada inválida
    {
        printf("Opção inválida!\n\n");
        return config(); // Chama a função novamente
    }

    switch (escolha)
    {
    case 1:
        config_temp();
        break;
    case 2:
        config_temp();
        break;
    case 3:
        config_temp();
        break;
    case 4:
        config_temp();
        break;
    case 5:
        config();
        break;
    }
}

int return_config()
{
    return config();
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

        char entrada = getchar();    // Captura um único caractere
        int escolha = entrada - '0'; // Converte de char para inteiro

        if (!(escolha >= 1 && escolha <= 5)) // Verifica entrada inválida
        {
            printf("Opção inválida!\n\n");
            return config(); // Chama a função novamente
        }

        switch (escolha)
        {
        case 1:
            config_temp();
            break;
        case 2:
            // Código das configurações gerais
            break;
        case 3:
            // Verifica se as configurações já estão nos valores de fábrica
            if (verificar_config_fabrica())
            {
                printf("As configurações já estão nos valores de fábrica:\n");
                printf("Temperatura mínima para exibir alerta: %.2fºC\n", temp_min);
                printf("Temperatura máxima para exibir alerta: %.2fºC\n", temp_max);
                printf("Valor do incremento em relação às temperaturas máxima e mínima que serão simuladas: %.2fºC\n", incremento);

                printf("\nEnvie qualquer caractere para continuar...\n");

                // Espera o usuário pressionar Enter
                getchar(); // Captura o Enter

                // Retorna as configurações de fábrica (caso precise de algo específico, adicione aqui)
                printf("Voltando às configurações...\n");

                return_config();
            }
            else
            {
                // Se não estiver, faz o reset
                printf("As configurações não estão nos valores de fábrica.\nReconfigurando...\n");
                sleep_ms(2000);
                reset_config_fabrica();
                return_config();
            }
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
        pwm_set_gpio_level(VERMELHO, 2048);
        limpar();
        display("Ative o Serial", 8, 10);
        display("Monitor", 36, 22);
        display("e tente", 36, 34);
        display("novamente", 28, 46);
        sleep_ms(4000);
        pwm_set_gpio_level(VERMELHO, 0);
    }

    menu_init(); // configura novamente os botões do menu
    return 0;
}

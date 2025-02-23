#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "display.h"
#include "defines.h"
#include "config_uart.h"
#include "init.h"
#include "pico/bootrom.h"
#include "config_uart_functions.h"

// Valores de fábrica
const float temp_min_fabrica = 2.0;
const float temp_max_fabrica = 8.0;
const float incremento_fabrica = 5.0;

// Váriaveis globais configuráveis do projeto
volatile float temp_min = temp_min_fabrica;
volatile float temp_max = temp_max_fabrica;
volatile float incremento = incremento_fabrica;

volatile float temp_min_temporario = temp_min_fabrica;
volatile float temp_max_temporario = temp_max_fabrica;
volatile float incremento_temporario = incremento_fabrica;

volatile float temp_crit_min;
volatile float temp_crit_max;

void save_and_quit()
{
    if (!(temp_min_temporario >= temp_max_temporario))
    {
        temp_min = temp_min_temporario;
        temp_max = temp_max_temporario;
        incremento = incremento_temporario;

        config();
    } else {
        printf("Valores estão apresentando erros, redefinindo para os valores de fábrica...\n");
        reset_config_fabrica();
        config_temp();
    }
}

void no_save()
{
    temp_min_temporario = temp_min_fabrica;
    temp_max_temporario = temp_max_fabrica;
    incremento_temporario = incremento_fabrica;
    config();
}

// Função responsável por alterar valores
int alterar_valor(int alterar)
{
    float valor;
    printf("Digite o novo valor: ");
    if (scanf("%f", &valor) == 1)
    { // Verifica se a leitura de um float foi bem-sucedida
        switch (alterar)
        {
        case 1:
            temp_max_temporario = valor;
            printf("Temperatura máxima temporária alterada de %.2f ºC para: %.2f ºC\n", temp_max, temp_max_temporario);
            return config_temp();
            break;
        case 2:
            temp_min_temporario = valor;
            printf("Temperatura mínima temporária alterada de %.2f ºC para: %.2f ºC\n", temp_min, temp_min_temporario);
            return config_temp();
            break;
        case 3:
            incremento_temporario = valor;
            printf("Incremento temporário alterado de %.2f ºC para: %.2f ºC\n", incremento, incremento_temporario);
            return config_temp();
            break;
        }
    }
    else
    {
        printf("Valor inválido! Digite um número.\n");
        return alterar_valor(alterar);
    }
}

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

void menu_off(){
    pwm_set_gpio_level(VERMELHO, 2048);
    limpar();
    display("Ative o Serial", 8, 10);
    display("Monitor", 36, 22);
    display("e tente", 36, 34);
    display("novamente", 28, 46);
    sleep_ms(4000);
    pwm_set_gpio_level(VERMELHO, 0);
}


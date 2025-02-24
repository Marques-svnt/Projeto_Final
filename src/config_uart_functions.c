#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
const int tempo_config_fabrica = 1000;
const int unidade_relatorio_fabrica = 1;

// Váriaveis globais configuráveis do projeto
volatile float temp_min = temp_min_fabrica;
volatile float temp_max = temp_max_fabrica;
volatile float incremento = incremento_fabrica;

volatile float temp_min_temporario = temp_min_fabrica;
volatile float temp_max_temporario = temp_max_fabrica;
volatile float incremento_temporario = incremento_fabrica;

volatile float temp_crit_min;
volatile float temp_crit_max;

volatile int unidade_relatorio = unidade_relatorio_fabrica;
volatile int tempo_config = tempo_config_fabrica;

volatile int unidade_relatorio_temporario = unidade_relatorio_fabrica;
volatile int tempo_config_temporario = tempo_config_fabrica;

bool gerar_relatorio = 1;

void save_and_quit_temp()
{
    if (!(temp_min_temporario >= temp_max_temporario))
    {
        temp_min = temp_min_temporario;
        temp_max = temp_max_temporario;
        incremento = incremento_temporario;

        config();
    }
    else
    {
        printf("Valores estão apresentando erros, redefinindo para os valores de fábrica...\n");
        reset_config_fabrica();
        config_temp();
    }
}

void no_save_temp()
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
    gerar_relatorio = 1;
    unidade_relatorio = unidade_relatorio_fabrica;
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

void menu_off()
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

void mudar_gerar_relatorio()
{
    {
        gerar_relatorio = !gerar_relatorio;
        config_relatorio();
    }
}

int mudar_unidade_relatorio()
{
    int escolha;
    printf("1 - Celsius\n");
    printf("2 - Kelvin\n");
    printf("3 - Fahrenheit\n");
    scanf("%d", &escolha);
    if (!(escolha >= 1 && escolha <= 3)) // Verifica entrada inválida
    {
        printf("Opção inválida!\n\n");
        return mudar_unidade_relatorio(); // Chama a função novamente
    }
    switch (escolha)
    {
    case 1:
        unidade_relatorio_temporario = 1;
        config_relatorio();
        break;
    case 2:
        unidade_relatorio_temporario = 2;
        config_relatorio();
        break;
    case 3:
        unidade_relatorio_temporario = 3;
        config_relatorio();
        break;
    }
}

int mudar_intervalo_relatório()
{
    int valor;
    printf("Digite o novo valor para o intervalo dos dados em segundos: ");
    if (scanf("%i", &valor) == 1)
    { // Verifica se a leitura de um float foi bem-sucedida
        printf("\nValor alterado de %i segundos para %i segundos\n", (tempo_config_temporario / 1000), valor);
        tempo_config_temporario = valor * 1000;
        return config_relatorio();
    }
    else
    {
        printf("Valor inválido! Digite um número.\n");
        return mudar_intervalo_relatório();
    }
}

void save_and_quit_relatorio()
{
    tempo_config = tempo_config_temporario;
    unidade_relatorio = unidade_relatorio_temporario;
    config();
}

void no_save_relatorio()
{
    gerar_relatorio = 1;
    tempo_config_temporario = tempo_config_fabrica;
    unidade_relatorio_temporario = unidade_relatorio_fabrica;
    config();
}
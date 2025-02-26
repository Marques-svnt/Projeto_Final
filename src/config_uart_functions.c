// Bibliotecas padrão em C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "pico/bootrom.h"

// Headers do projeto
#include "config_uart.h"
#include "config_uart_functions.h"
#include "defines.h"
#include "display.h"
#include "init.h"

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

// Função para salvar as configurações temporárias e sair
int save_and_quit_temp()
{
    if (!(temp_min_temporario >= temp_max_temporario) && !(temp_max_temporario - temp_min_temporario <= incremento_temporario))
    {
        temp_min = temp_min_temporario;
        temp_max = temp_max_temporario;
        incremento = incremento_temporario;

        return config();
    }
    else
    {
        printf("Valores estão apresentando erros, redefinindo para os valores de fábrica...\n");
        temp_min_temporario = temp_min_fabrica;
        temp_max_temporario = temp_max_fabrica;
        return config_temp();
    }
}

// Função para não salvar as alterações temporárias e retornar
int no_save_temp()
{
    temp_min_temporario = temp_min;
    temp_max_temporario = temp_max;
    incremento_temporario = incremento;
    return config();
}

// Função para alterar valores temporários, baseado na escolha
int alterar_valor(int alterar)
{
    float valor;

    while (1)
    {
        valor = ler_float_seguro(); // Chama a função segura para obter um float

        // Atualiza o valor temporário com base na escolha
        switch (alterar)
        {
        case 1:
            temp_max_temporario = valor;
            printf("Temperatura máxima temporária alterada de %.2f ºC para: %.2f ºC\n", temp_max, temp_max_temporario);
            break;
        case 2:
            temp_min_temporario = valor;
            printf("Temperatura mínima temporária alterada de %.2f ºC para: %.2f ºC\n", temp_min, temp_min_temporario);
            break;
        case 3:
            incremento_temporario = valor;
            printf("Incremento temporário alterado de %.2f ºC para: %.2f ºC\n", incremento, incremento_temporario);
            break;
        }
        break; // Sai do loop quando a entrada for válida
    }

    return config_temp(); // Retorna à função de configuração de temperatura
}

// Função para resetar as configurações para os valores de fábrica
void reset_config_fabrica()
{
    temp_min = temp_min_fabrica;
    temp_min_temporario = temp_min_fabrica;
    temp_max = temp_max_fabrica;
    temp_max_temporario = temp_max_fabrica;
    incremento = incremento_fabrica;
    incremento_temporario = incremento_fabrica;
    gerar_relatorio = 1;
    unidade_relatorio = unidade_relatorio_fabrica;
    unidade_relatorio_temporario = unidade_relatorio_fabrica;
    tempo_config = tempo_config_fabrica;
    tempo_config_temporario = tempo_config_fabrica;
    printf("Configurações de fábrica restauradas.\n");
}

// Função para verificar se as configurações estão no valor de fábrica
bool verificar_config_fabrica()
{
    if (temp_min == temp_min_fabrica && temp_max == temp_max_fabrica && incremento == incremento_fabrica && gerar_relatorio == 1 && unidade_relatorio == unidade_relatorio_fabrica && tempo_config == tempo_config_fabrica)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Função para caso o monitor serial esteja desconectado
void usb_off()
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

// Função para alternar o estado de geração de relatório
int mudar_gerar_relatorio()
{
    gerar_relatorio = !gerar_relatorio;
    return config_relatorio();
}

// Função para mudar a unidade de medida do relatório
int mudar_unidade_relatorio()
{
    int escolha;
    while (1)
    {
        printf("1 - Celsius\n");
        printf("2 - Kelvin\n");
        printf("3 - Fahrenheit\n");
        printf("Escolha uma opção: ");

        escolha = ler_inteiro_seguro(); // Usa a função para garantir entrada válida

        if (escolha >= 1 && escolha <= 3)
        {
            break;
        }

        printf("Opção inválida! Tente novamente.\n\n");
    }
    switch (escolha)
    {
    case 1:
        unidade_relatorio_temporario = 1;
        break;
    case 2:
        unidade_relatorio_temporario = 2;
        break;
    case 3:
        unidade_relatorio_temporario = 3;
        break;
    }
    return config_relatorio();
}

// Função para mudar o intervalo de tempo do relatório
int mudar_intervalo_relatorio()
{
    int valor;

    while (1)
    {
        printf("Digite o novo valor para o intervalo dos dados em segundos: ");
        valor = ler_inteiro_seguro(); // Garante entrada válida

        // Verifica se o valor é positivo (intervalo de tempo não pode ser negativo)
        if (valor >= 0)
        {
            printf("\nValor alterado de %i segundos para %i segundos\n", (tempo_config_temporario / 1000), valor);
            tempo_config_temporario = valor * 1000; // Converte segundos para milissegundos
            break;
        }
        else
        {
            printf("Valor inválido! O intervalo não pode ser negativo.\n");
        }
    }

    return config_relatorio();
}

// Função para salvar as configurações de relatório e sair
int save_and_quit_relatorio()
{
    tempo_config = tempo_config_temporario;
    unidade_relatorio = unidade_relatorio_temporario;
    return config();
}

// Função para não salvar as configurações temporárias do relatório
int no_save_relatorio()
{
    gerar_relatorio = 1;
    tempo_config_temporario = tempo_config;
    unidade_relatorio_temporario = unidade_relatorio;
    return config();
}

// Função para exibir as configurações atuais
int show_config()
{
    printf("Temperatura mínima atual: %.2f\n", temp_min);
    printf("Temperatura máxima atual: %.2f\n", temp_max);
    printf("Incremento da temperatura atual: %.2f\n", incremento);
    printf("Gerar relatórios: ");
    if (gerar_relatorio)
    {
        printf("Ligado\n");
    }
    else
    {
        printf("Desligado\n");
    }
    printf("Unidade de medida do relatório atual: ");
    switch (unidade_relatorio)
    {
    case 1:
        printf("Celsius\n");
        break;
    case 2:
        printf("Kelvin\n");
        break;
    case 3:
        printf("Fahrenheit\n");
        break;
    }
    printf("Intervalo dos dados do relatório atual: %i\n", tempo_config);
    printf("\nEnvie qualquer caractere para continuar...\n");

    getchar();

    printf("Voltando às configurações...\n");

    return config();
}

// Código que garante que o número lido não cause overflow
int ler_inteiro_seguro()
{
    char entrada[20]; // Buffer para armazenar a entrada do usuário
    char *endptr;
    long valor;

    while (1)
    {
        printf("Digite um número: ");
        if (!fgets(entrada, sizeof(entrada), stdin))
        {
            printf("Erro na leitura.\n");
            continue;
        }

        entrada[strcspn(entrada, "\n")] = 0;

        valor = strtol(entrada, &endptr, 10);

        if (endptr == entrada || *endptr != '\0')
        {
            printf("Entrada inválida! Digite um número válido.\n");
            continue;
        }
        if (valor < INT_MIN || valor > INT_MAX)
        {
            printf("Número fora do intervalo permitido.\n");
            continue;
        }

        return (int)valor;
    }
}

// Código que garante que o número lido não cause overflow
float ler_float_seguro()
{
    char entrada[50];
    char *endptr;
    float valor;

    while (1)
    {
        printf("Digite o novo valor: ");

        if (!fgets(entrada, sizeof(entrada), stdin))
        {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }
        entrada[strcspn(entrada, "\n")] = 0;

        valor = strtof(entrada, &endptr);

        if (endptr == entrada || *endptr != '\0')
        {
            printf("Valor inválido! Digite um número.\n");
            continue;
        }

        return valor;
    }
}

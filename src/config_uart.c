// Bibliotecas padrão em C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "hardware/pwm.h"
#include "pico/bootrom.h"
#include "pico/stdlib.h"

// Headers do projeto
#include "config_uart.h"
#include "config_uart_functions.h"
#include "defines.h"
#include "display.h"
#include "init.h"

// Função da configuração de relatório
int config_relatorio()
{
    int escolha;
    while (1)
    {
        printf("\n1 - Ligar/Desligar geração de relatórios (Atualmente:");
        printf(gerar_relatorio ? " Ligado)\n" : " Desligado)\n");

        printf("2 - Mudar unidade de medida (Atualmente:");
        switch (unidade_relatorio_temporario)
        {
        case 1:
            printf(" Celsius)\n");
            break;
        case 2:
            printf(" Kelvin)\n");
            break;
        case 3:
            printf(" Fahrenheit)\n");
            break;
        }

        printf("3 - Mudar intervalo dos dados (Atualmente: %is)\n", (tempo_config_temporario / 1000));
        printf("4 - Salvar configurações e sair\n");
        printf("5 - Sair sem salvar\n");

        escolha = ler_inteiro_seguro(); // Uso da função segura

        if (escolha >= 1 && escolha <= 5)
        {
            break;
        }

        printf("Opção inválida! Tente novamente.\n\n");
    }

    // Executa a ação com base na escolha do usuário
    switch (escolha)
    {
    case 1:
        mudar_gerar_relatorio();
        break;
    case 2:
        mudar_unidade_relatorio();
        break;
    case 3:
        mudar_intervalo_relatorio();
        break;
    case 4:
        save_and_quit_relatorio();
        break;
    case 5:
        no_save_relatorio();
        break;
    }

    return 0;
}

// Função da configuração de temperatura
int config_temp()
{
    int escolha;
    while (1)
    {
        printf("\n1 - Alterar valor da temperatura máxima (Atualmente: %.2fºC)\n", temp_max_temporario);
        printf("2 - Alterar valor da temperatura mínima (Atualmente: %.2fºC)\n", temp_min_temporario);
        printf("3 - Alterar valor do incremento (Atualmente: %.2fºC)\n", incremento_temporario);
        printf("4 - Salvar configurações e sair\n");
        printf("5 - Sair sem salvar\n");

        escolha = ler_inteiro_seguro(); // Uso da função segura

        if (escolha >= 1 && escolha <= 5)
        {
            break;
        }

        printf("Opção inválida! Tente novamente.\n\n");
    }

    // Executa a ação com base na escolha do usuário
    switch (escolha)
    {
    case 1:
    case 2:
    case 3:
        alterar_valor(escolha);
        break;
    case 4:
        save_and_quit_temp();
        break;
    case 5:
        no_save_temp();
        break;
    }

    return 0;
}

// Função geral que coordena as configurações
int config()
{
    int exit = 0;
    if (stdio_usb_connected())
    {
        display("Configurando", 16, 25);
        display("o projeto", 28, 35);

        int escolha;
        while (1)
        {
            printf("\n\n===================================================================Configurações===================================================================\n\n");
            printf("1 - Configurações do sensor de temperatura\n");
            printf("2 - Configurações de relatório\n");
            printf("3 - Restaurar configurações de fábrica\n");
            printf("4 - Mostrar configurações atuais do projeto\n");
            printf("5 - Entrar em bootsel\n");
            printf("6 - Voltar ao menu\n");

            escolha = ler_inteiro_seguro(); // Uso da função segura

            if (escolha >= 1 && escolha <= 6)
            {
                break;
            }

            printf("Opção inválida! Tente novamente.\n\n");
        }

        // Executa a ação com base na escolha do usuário
        switch (escolha)
        {
        case 1:
            config_temp();
            break;
        case 2:
            config_relatorio();
            break;
        case 3:
            if (verificar_config_fabrica())
            {
                printf("As configurações já estão nos valores de fábrica:\n");
                printf("Temperatura mínima para exibir alerta: %.2fºC\n", temp_min_fabrica);
                printf("Temperatura máxima para exibir alerta: %.2fºC\n", temp_max_fabrica);
                printf("Valor do incremento em relação às temperaturas máxima e mínima que serão simuladas: %.2fºC\n", incremento_fabrica);
                printf("Gerando Relatório\n");
                printf("Unidade de medida: Celsius\n");
                printf("Tempo entre leitura do relatório: %i segundos\n", (tempo_config_fabrica / 1000));

                printf("Voltando às configurações...\n");
                sleep_ms(2000);
            }
            else
            {
                printf("As configurações não estão nos valores de fábrica.\nReconfigurando...\n");
                sleep_ms(2000);
                reset_config_fabrica();
            }
            return config();
        case 4:
            show_config();
            break;
        case 5:
            limpar();
            display("Entrando em", 20, 22);
            display("BOOTSEL", 36, 32);
            sleep_ms(2000);
            limpar();
            reset_usb_boot(0, 0); // Entra em modo bootsel
            break;
        case 6:
            exit = 1;
            break;
        }
    }
    else
    {
        usb_off(); // Exibe uma mensagem caso o monitor serial esteja desconectado
    }
    if (exit == 1)
    {
        printf("\n\n==============================================================Fechando Configurações==============================================================\n\n");
        exit = 0;
    }
    menu_init(); // configura novamente os botões do menu
    return 0;
}
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

int config_relatorio()
{
    int escolha;
    printf("1 - Ligar/Desligar geração de relatórios (Atualmente:");
    if (gerar_relatorio)
    {
        printf(" Ligado)\n");
    }
    else
    {
        printf(" Desligado)\n");
    }
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

    scanf("%d", &escolha); // Captura um número inteiro

    if (!(escolha >= 1 && escolha <= 5)) // Verifica entrada inválida
    {
        printf("Opção inválida!\n\n");
        return config_temp(); // Chama a função novamente
    }
    switch (escolha)
    {
    case 1:
        mudar_gerar_relatorio();
        break;
    case 2:
        mudar_unidade_relatorio();
        break;
    case 3:
        mudar_intervalo_relatório();
        break;
    case 4:
        save_and_quit_relatorio();
        break;
    case 5:
        no_save_relatorio();
        break;
    }
}

int config_temp()
{
    int escolha;
    printf("1 - Alterar valor da temperatura máxima (Atualmente: %.2fºC)\n", temp_max_temporario);
    printf("2 - Alterar valor da temperatura mínima (Atualmente: %.2fºC)\n", temp_min_temporario);
    printf("3 - Alterar valor do incremento (Atualmente: %.2fºC)\n", incremento_temporario);
    printf("4 - Salvar configurações e sair\n");
    printf("5 - Sair sem salvar\n");

    scanf("%d", &escolha); // Captura um número inteiro

    if (!(escolha >= 1 && escolha <= 5)) // Verifica entrada inválida
    {
        printf("Opção inválida!\n\n");
        return config_temp(); // Chama a função novamente
    }
    switch (escolha)
    {
    case 1:
        alterar_valor(escolha);
        break;
    case 2:
        alterar_valor(escolha);
        break;
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
}

int config()
{
    if (stdio_usb_connected())
    {
        int escolha;
        printf("\n\n===================================================================Configurações===================================================================\n\n");
        printf("1 - Configurações do sensor de temperatura\n");
        printf("2 - Configurações de relatório\n");
        printf("3 - Restaurar configurações de fábrica\n");
        printf("4 - Entrar em bootsel\n");
        printf("5 - Voltar ao menu\n");
        printf("Escolha uma opção: ");

        scanf("%d", &escolha); // Captura um número inteiro

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
            config_relatorio();
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

                config();
            }
            else
            {
                // Se não estiver, faz o reset
                printf("As configurações não estão nos valores de fábrica.\nReconfigurando...\n");
                sleep_ms(2000);
                reset_config_fabrica();
                config();
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
        menu_off();
    }
    printf("\n\n==============================================================Fechando Configurações==============================================================\n\n");
    menu_init(); // configura novamente os botões do menu
    return 0;
}

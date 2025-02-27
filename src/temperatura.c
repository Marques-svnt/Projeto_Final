// Bibliotecas padrão em C
#include <stdio.h>
#include <string.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Headers do projeto
#include "buzzer_temp.h"
#include "config_uart_functions.h"
#include "converts.h"
#include "defines.h"
#include "display.h"
#include "init.h"
#include "interrupt.h"
#include "pio.h"
#include "relatorio.h"

// Variáveis globais
volatile int unid = 0;          // Unidade de medida: 0 - Celsius, 1 - Kelvin, 2 - Fahrenheit
volatile int exec = 1;          // Flag de execução: 1 - executando, 0 - sair
static volatile uint32_t last_time_A = 0; // Tempo do último evento do botão A (em microssegundos)
static volatile uint32_t last_time_B = 0; // Tempo do último evento do botão B
volatile float temp;            // Temperatura medida
extern volatile bool alarme_ativo; // Flag do alarme de temperatura
bool titulo = 1;                // Controla a exibição do título
uint16_t vry_value;             // Valor do ADC (leitura do sensor)
char temp_str[10];              // Buffer para armazenar a string de temperatura

// Função para controlar os LEDs com base na temperatura
void controlar_leds(float temperatura, float temp_min, float temp_max, float temp_crit_max, float temp_crit_min)
{
    // Se a temperatura ultrapassar o valor máximo ou mínimo, aciona o LED vermelho
    if (temperatura > temp_max)
    {
        if (temperatura > (temp_crit_max - 0.5)) // Brilho mais intenso se for crítico
        {
            pwm_set_gpio_level(VERMELHO, 4096);
            pwm_set_gpio_level(VERDE, 0);
        }
        else
        {
            pwm_set_gpio_level(VERMELHO, 1024);
            pwm_set_gpio_level(VERDE, 0);
        }
    }
    else if (temperatura < temp_min)
    {
        if (temperatura < (temp_crit_min + 0.5)) // Brilho mais intenso se for crítico
        {
            pwm_set_gpio_level(VERMELHO, 4096);
            pwm_set_gpio_level(VERDE, 0);
        }
        else
        {
            pwm_set_gpio_level(VERMELHO, 1024);
            pwm_set_gpio_level(VERDE, 0);
        }
    }
    else
    {
        pwm_set_gpio_level(VERMELHO, 0);
        pwm_set_gpio_level(VERDE, 1024);
    }
}

// Função para simular a leitura do sensor de temperatura via ADC
void simular_adc_temp()
{
    // Configura o input do ADC e lê o valor
    adc_select_input(1);
    sleep_us(5); // Delay para estabilidade
    vry_value = adc_read();

    temp_crit_max = temp_max + incremento;
    temp_crit_min = temp_min - incremento;

    // Converte o valor do ADC para a temperatura com base na unidade selecionada
    switch (unid)
    {
    case 0: // Celsius
        set_one_led(10, 0, 0, 20); // Exibe a unidade de medida na matriz

        temp = converter_adc_para_temp(vry_value); // Converte o ADC para temperatura em Celsius

        controlar_leds(temp, temp_min, temp_max, temp_crit_max, temp_crit_min); // Controla os LEDs com base na temperatura
        alarme_crit(temp, temp_min, temp_max); // Verifica se a temperatura está fora dos limites e aciona o alarme

        display_set_temp("Celsius:", 36, 10);
        break;

    case 1: // Kelvin
        set_one_led(11, 0, 0, 20);

        temp = celsius_para_kelvin(converter_adc_para_temp(vry_value)); // Converte para Kelvin

        controlar_leds(temp, celsius_para_kelvin(temp_min), celsius_para_kelvin(temp_max),celsius_para_kelvin(temp_crit_max),celsius_para_kelvin(temp_crit_min));

        alarme_crit(temp, celsius_para_kelvin(temp_min), celsius_para_kelvin(temp_max));

        display_set_temp("Kelvin:", 40, 10);
        break;

    case 2: // Fahrenheit
        set_one_led(12, 0, 0, 20);

        temp = celsius_para_fahrenheit(converter_adc_para_temp(vry_value)); // Converte para Fahrenheit

        controlar_leds(temp, celsius_para_fahrenheit(temp_min), celsius_para_fahrenheit(temp_max),celsius_para_fahrenheit(temp_crit_max), celsius_para_fahrenheit(temp_crit_min));

        alarme_crit(temp, celsius_para_fahrenheit(temp_min), celsius_para_fahrenheit(temp_max));

        display_set_temp("Fahrenheit:", 24, 10);
        break;
    }

    // Converte a temperatura para string com 2 casas decimais
    converter_float_para_string(temp, temp_str, 2); 

    // Adiciona a unidade de medida à string de temperatura
    if (unid == 0)
    {
        strcat(temp_str, "*C ");
    }
    else if (unid == 1)
    {
        strcat(temp_str, "K ");
    }
    else if (unid == 2)
    {
        strcat(temp_str, "*F ");
    }

    // Exibe a temperatura no display
    display_set_temp(temp_str, 40, 25);
}

// Função de interrupção para manipular a troca de unidades e saída do programa
void gpio_irq_handler_temp(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Troca a unidade de medida ao pressionar o botão A
    if (gpio == BUTTON_A && debounce(&last_time_A, 200000))
    {
        last_time_A = current_time;
        unid = (unid + 1) % 3; // Alterna entre 0, 1 e 2 (Celsius, Kelvin, Fahrenheit)
        display_set_temp("             ", 0, 10); // Limpa o display
        limpar(); // Limpa os dados exibidos
    }
    // Volta para o menu ao pressionar o botão B
    else if (gpio == BUTTON_B && debounce(&last_time_B, 300000))
    {
        last_time_B = current_time;
        exec = 0; // Interrompe a execução
        alarme_ativo = false; // Desliga o alarme

        uint slice_num = pwm_gpio_to_slice_num(BUZZER);
        uint channel = pwm_gpio_to_channel(BUZZER);
        pwm_set_enabled(slice_num, false); // Desativa o PWM do buzzer

        pwm_set_gpio_level(VERMELHO, 0); // Desativa os LEDs
        pwm_set_gpio_level(VERDE, 0);

        limpar(); // Limpa os dados
    }
}

// Função principal de controle da temperatura
int temperatura()
{
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_temp);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_temp);

    while (exec == 1)
    {
        if (gerar_relatorio)
        {
            registrar_temperatura(temp, temp_min, temp_max, vry_value); // Registra a temperatura no relatório
        }
        simular_adc_temp(); // Simula a leitura do ADC e controla os LEDs
        sleep_ms(50); // Atraso para evitar sobrecarga de processamento
    }
    if(!titulo)
    {
        printf("================================================================================================================================================\n\n");
        titulo = !titulo; // Imprime o título uma vez
    }
    menu_init(); // Reconfigura os botões para o menu
    exec = 1;    // Retorna ao código principal
    return 0;
}

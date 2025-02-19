#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "defines.h"
#include "converts.h"
#include "pio.h"
#include "init.h"
#include "display.h"
#include "interrupt.h"
#include "pwm.h"

volatile int unid = 0; // Unidade de medida 0 - Celsius 1 - Kelvin 2 - Fahrenheit
volatile int exec = 1; /* 1 - Enquanto o código estiver executando
                          0 - Enquanto quiser sair*/

static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time_B = 0;
static volatile uint32_t last_time_J = 0;

float temp;
static volatile float temp_min = 2.0;
static volatile float temp_max = 8.0;
float temp_crit_min;
float temp_crit_max;

char temp_str[10]; // Buffer para armazenar a string

void controlar_leds(float temperatura, float temp_min, float temp_max)
{
    if (temperatura > temp_max)
    {
        pwm_set_gpio_level(VERMELHO, 2 * (temperatura - temp_max) * 100);
    }
    else if (temperatura < temp_min)
    {
        pwm_set_gpio_level(AZUL, 2 * (temp_min - temperatura) * 100);
    }
    else
    {
        pwm_set_gpio_level(VERMELHO, 0);
        pwm_set_gpio_level(AZUL, 0);
    }
}

void simular_adc_temp()
{
    // Configura o input do ADC
    adc_select_input(1);
    sleep_us(5); // Pequeno delay para estabilidade
    uint16_t vry_value = adc_read();

    float incremento_min = calcular_incremento(temp_min);
    float incremento_max = calcular_incremento(temp_max);

    temp_crit_min = temp_min - incremento_min;
    temp_crit_max = temp_max + incremento_max;

    // Converter dados digitais do ADC para os parâmetros de temperatura
    switch (unid)
    {
    case 0: // Celsius
        set_one_led(3, 0, 0, 20);
        temp = converter_adc_para_temp(vry_value);
        controlar_leds(temp, temp_min, temp_max);
        break;

    case 1: // Kelvin
        set_one_led(4, 0, 0, 20);
        temp = celsius_para_kelvin(converter_adc_para_temp(vry_value));
        controlar_leds(temp, celsius_para_kelvin(temp_min), celsius_para_kelvin(temp_max));
        break;

    case 2: // Fahrenheit
        set_one_led(5, 0, 0, 20);
        temp = celsius_para_fahrenheit(converter_adc_para_temp(vry_value));
        controlar_leds(temp, celsius_para_fahrenheit(temp_min), celsius_para_fahrenheit(temp_max));
        break;
    }
    converter_float_para_string(temp, temp_str, 2); // Converte para string com 1 casa decimal
    display(temp_str, 45, 20);
}

void gpio_irq_handler_temp(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Troca a unidade de medida
    if (gpio == BUTTON_A && debounce(&last_time_A, 200000))
    {
        last_time_A = current_time;
        // Altera o valor da unidade ao apertar o botao
        unid = (unid + 1) % 3; // Alterna entre 0, 1 e 2
        limpar();              // Limpa apenas quando necessário
    }
    // Volta para o menu
    else if (gpio == BUTTON_B && debounce(&last_time_B, 200000))
    {
        last_time_B = current_time;
        exec = 0;
    }
}

int temperatura()
{
    printf("estou na temp");
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_temp);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler_temp);

    while (exec == 1)
    {
        simular_adc_temp();
        sleep_ms(50);
    }
    menu_init(); // configura novamente os botões do menu
    exec = 1;    // permite que volte ao código após sair dele uma vez
    return 0;
}

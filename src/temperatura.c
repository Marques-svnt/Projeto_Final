#include <stdio.h>
#include "pico/stdlib.h"
#include <string.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "defines.h"
#include "converts.h"
#include "pio.h"
#include "buzzer_temp.h"
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

volatile float temp;
volatile float temp_min = 2.0;
volatile float temp_max = 8.0;
volatile float incremento = 5.0;
float temp_crit_min;
float temp_crit_max;

extern volatile bool alarme_ativo;

char temp_str[10]; // Buffer para armazenar a string

void controlar_leds(float temperatura, float temp_min, float temp_max)
{
    if (temperatura > temp_max)
    {
        if (temperatura > (temp_crit_max - 0.5)) // Brilho mais intenso caso seja muito crítico
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
        if (temperatura < (temp_crit_min + 0.5))
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

void simular_adc_temp()
{
    // Configura o input do ADC
    adc_select_input(1);
    sleep_us(5); // Pequeno delay para estabilidade
    uint16_t vry_value = adc_read();

    temp_crit_min = temp_min - incremento;
    temp_crit_max = temp_max + incremento;

    // Converter dados digitais do ADC para os parâmetros de temperatura
    switch (unid)
    {
    case 0:                        // Celsius
        set_one_led(10, 0, 0, 20); // Exibi a unidade de medida na matriz

        temp = converter_adc_para_temp(vry_value); // Converte o valor em ADC para a temperatura

        controlar_leds(temp, temp_min, temp_max); // Configura os leds PWM

        alarme_crit(temp, temp_min, temp_max); // Configura o alarme

        display("Celsius:", 45, 10);
        break;

    case 1: // Kelvin
        set_one_led(11, 0, 0, 20);

        temp = celsius_para_kelvin(converter_adc_para_temp(vry_value));

        controlar_leds(temp, celsius_para_kelvin(temp_min), celsius_para_kelvin(temp_max));

        alarme_crit(temp, celsius_para_kelvin(temp_min), celsius_para_kelvin(temp_max));

        display("Kelvin:", 45, 10);
        break;

    case 2: // Fahrenheit
        set_one_led(12, 0, 0, 20);

        temp = celsius_para_fahrenheit(converter_adc_para_temp(vry_value));

        controlar_leds(temp, celsius_para_fahrenheit(temp_min), celsius_para_fahrenheit(temp_max));

        alarme_crit(temp, celsius_para_fahrenheit(temp_min), celsius_para_fahrenheit(temp_max));

        display("Fahrenheit:", 45, 10);
        break;
    }

    converter_float_para_string(temp, temp_str, 2); // Converte para string com 1 casa decimal
    strcat(temp_str," *C "); // Garante que um número extra não vá aparecer caso haja deslocamento com sinal de negativo ou decimais maiores
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
    else if (gpio == BUTTON_B && debounce(&last_time_B, 300000))
    {
        last_time_B = current_time;
        exec = 0;
        alarme_ativo = false;            // flag que desliga o som do buzzer
        buzzer_stop();                   // garante que o buzzer desliga
        pwm_set_gpio_level(VERMELHO, 0); // Garante que os leds não ficarão acessos
        pwm_set_gpio_level(VERDE, 0);

        limpar(); // Limpa os dados
    }
}

int temperatura()
{
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

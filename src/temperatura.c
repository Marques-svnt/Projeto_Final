#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "defines.h"
#include "init.h"
#include "interrupt.h"
#include "pwm.h"

volatile int exec = 1; /* 1 - Enquanto o código estiver executando
                          0 - Enquanto quiser sair*/

static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time_B = 0;
static volatile uint32_t last_time_J = 0;

static volatile float temp_min = 20.0;
static volatile float temp_max = 80.0;
float temp_crit_min;
float temp_crit_max;


void gpio_irq_handler_temp(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Troca a unidade de medida
    if (gpio == BUTTON_A && debounce(&last_time_A, 200000))
    {
        last_time_A = current_time;
    }
    // Volta para o menu
    else if (gpio == BUTTON_B && debounce(&last_time_B, 200000))
    {
        last_time_B = current_time;
        exec = 0;
    }
}

// Função para calcular o incremento proporcional
float calcular_incremento(float valor) {
    float ordem_grandeza = pow(10, floor(log10(valor)));  // Exemplo: 5 → 1, 50 → 10, 500 → 100
    return 2.0 * ordem_grandeza;  // Ajuste de escala (2x a ordem de grandeza)
}

// Converter o valor do ADC para temperatura
float converter_adc_para_temp(uint16_t adc_value) {
    return temp_crit_min + ((adc_value / 4096.0) * (temp_crit_max - temp_crit_min));
}

void simular_adc_temp()
{
    // Configura o input do ADC
    adc_select_input(1);
    sleep_us(5);                     // Pequeno delay para estabilidade
    uint16_t vry_value = adc_read(); 

    float incremento_min = calcular_incremento(temp_min);
    float incremento_max = calcular_incremento(temp_max);

    temp_crit_min = temp_min - incremento_min;
    temp_crit_max = temp_max + incremento_max;

    // Converter dados digitais do ADC para os parâmetros de temperatura
    float temperatura = converter_adc_para_temp(vry_value);  // Conversão para temperatura

    printf("%.f",temperatura);
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
    menu_init();
    return 0;
}

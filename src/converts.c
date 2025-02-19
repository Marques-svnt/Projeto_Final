#include <stdio.h>
#include "pico/stdlib.h"
#include <math.h>
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "defines.h"
#include "init.h"
#include "display.h"
#include "interrupt.h"
#include "pwm.h"

extern float temp_crit_min;
extern float temp_crit_max;

// Função para calcular o incremento proporcional
float calcular_incremento(float valor)
{
    float ordem_grandeza = pow(10, floor(log10(valor))); // Exemplo: 5 → 1, 50 → 10, 500 → 100
    return 2.0 * ordem_grandeza;                         // Ajuste de escala (2x a ordem de grandeza)
}

// Converter o valor do ADC para temperatura
float converter_adc_para_temp(uint16_t adc_value)
{
    return temp_crit_min + ((adc_value / 4096.0) * (temp_crit_max - temp_crit_min));
}

void converter_float_para_string(float valor, char *buffer, int casas_decimais)
{
    sprintf(buffer, "%.*f", casas_decimais, valor);
}
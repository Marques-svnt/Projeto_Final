// Bibliotecas padrão em C
#include <stdio.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

// Headers do projeto
#include "defines.h"
#include "display.h"
#include "init.h"
#include "interrupt.h"
#include "pwm.h"

// Declaração de variáveis externas para os limites críticos de temperatura
extern float temp_crit_min; // Temperatura crítica mínima
extern float temp_crit_max; // Temperatura crítica máxima

// Função que converte o valor lido do ADC para uma temperatura
float converter_adc_para_temp(uint16_t adc_value)
{
    return temp_crit_min + ((adc_value / 4096.0) * (temp_crit_max - temp_crit_min));
}

// Função que converte um valor float para uma string
void converter_float_para_string(float valor, char *buffer, int casas_decimais)
{
    sprintf(buffer, "%.*f", casas_decimais, valor);
}

// Função para converter temperatura de Celsius para Kelvin
float celsius_para_kelvin(float celsius)
{
    return celsius + 273.15;
}

// Função para converter temperatura de Celsius para Fahrenheit
float celsius_para_fahrenheit(float celsius)
{
    return (celsius * 9.0 / 5.0) + 32.0;
}
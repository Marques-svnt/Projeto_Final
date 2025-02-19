#ifndef CONVERTS_H
#define CONVERTS_H

// Declaração da função de conversão da temperatura
float calcular_incremento(float valor);
float converter_adc_para_temp(uint16_t adc_value);
void converter_float_para_string(float valor, char *buffer, int casas_decimais);

#endif
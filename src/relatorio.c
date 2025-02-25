// Bibliotecas padrão em C
#include <stdio.h>
#include <time.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "pico/stdlib.h"

// Headers do projeto
#include "config_uart_functions.h"
#include "converts.h"

// Variáveis globais para controle da temperatura e status
float temperatura_anterior = 0.0;
bool estava_critica = false;
uint64_t last_time = 0; // Tempo da última medição

extern bool titulo;
extern volatile int unidade_relatorio; // 1: Celsius (padrão), 2: Kelvin, 3: Fahrenheit

// Obtém o horário atual formatado (HH:MM:SS)
void obter_horario_formatado(char *buffer, size_t tamanho) {
    time_t tempo_atual = time(NULL);
    struct tm tempo_info;
    localtime_r(&tempo_atual, &tempo_info);
    strftime(buffer, tamanho, "%H:%M:%S", &tempo_info);
}

// Registra a temperatura com base no intervalo de tempo configurado
void registrar_temperatura(float temp, float temp_min, float temp_max, uint16_t vry) {
    if (titulo) {
        printf("\n\n===================================================================Relatório===================================================================\n\n");
        titulo = !titulo;
    }

    uint64_t now = to_ms_since_boot(get_absolute_time()); // Tempo atual em ms
    uint64_t delay_ms = tempo_config; // tempo_config deve estar definido em algum header

    float value = converter_adc_para_temp(vry);

    if (now - last_time >= delay_ms) {
        char horario[9]; // Buffer para HH:MM:SS
        obter_horario_formatado(horario, sizeof(horario));

        // Conversão conforme a unidade selecionada
        float temperatura_convertida = value;
        float max_convertida = temp_max;
        float min_convertida = temp_min;
        const char *unidade_str = "ºC";

        switch (unidade_relatorio) {
            case 2: // Kelvin
                temperatura_convertida = celsius_para_kelvin(value);
                max_convertida = celsius_para_kelvin(temp_max);
                min_convertida = celsius_para_kelvin(temp_min);
                unidade_str = "K";
                break;
            case 3: // Fahrenheit
                temperatura_convertida = celsius_para_fahrenheit(value);
                max_convertida = celsius_para_fahrenheit(temp_max);
                min_convertida = celsius_para_fahrenheit(temp_min);
                unidade_str = "ºF";
                break;
            case 1: // Celsius (padrão)
            default:
                break;
        }

        float variacao = temperatura_convertida - temperatura_anterior;

        // Exibe mensagem de alerta se a temperatura estiver fora dos limites
        if (temperatura_convertida < min_convertida || temperatura_convertida > max_convertida) {
            if (!estava_critica) {
                printf("[%s] Temperatura crítica! Temperatura: %.2f%s, ultrapassou os limites. Variação: %.2f%s\n\n",
                       horario, temperatura_convertida, unidade_str, variacao, unidade_str);
                estava_critica = true;
            } else {
                printf("[%s] Temperatura ainda fora do limite. Temperatura: %.2f%s. Variação: %.2f%s\n\n",
                       horario, temperatura_convertida, unidade_str, variacao, unidade_str);
            }
        } else {
            if (estava_critica) {
                printf("[%s] Temperatura estava crítica em %.2f%s, porém estabilizou para %.2f%s\n\n",
                       horario, temperatura_anterior, unidade_str, temperatura_convertida, unidade_str);
                estava_critica = false;
            } else {
                printf("[%s] Temperatura normal: %.2f%s. Variação: %.2f%s\n\n",
                       horario, temperatura_convertida, unidade_str, variacao, unidade_str);
            }
        }

        temperatura_anterior = temperatura_convertida;
        last_time = now;
    }
}

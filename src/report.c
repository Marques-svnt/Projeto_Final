#include <stdio.h>
#include "pico/stdlib.h"
#include "config_uart_functions.h"
#include "converts.h"
#include <time.h>

// Variáveis globais para controle de temperatura anterior e status
float temperatura_anterior = 0.0;
bool estava_critica = false;
uint64_t last_time = 0; // Para controle de tempo

extern volatile int unidade_relatorio;

// Função para obter o horário formatado
void obter_horario_formatado(char *buffer, size_t tamanho)
{
    time_t tempo_atual = time(NULL);
    struct tm tempo_info;
    localtime_r(&tempo_atual, &tempo_info);
    strftime(buffer, tamanho, "%H:%M:%S", &tempo_info);
}

// Função para registrar a temperatura com a passagem de tempo real
void registrar_temperatura(float temp, float temp_min, float temp_max, uint16_t vry)
{
    uint64_t now = to_ms_since_boot(get_absolute_time()); // Tempo atual em milissegundos
    uint64_t delay_ms = tempo_config;

    float value = converter_adc_para_temp(vry);

    if (now - last_time >= delay_ms)
    {
        char horario[9]; // Buffer para armazenar HH:MM:SS
        obter_horario_formatado(horario, sizeof(horario));

        float temperatura_convertida = value;
        float max_convertida = temp_max;
        float min_convertida = temp_min;
        const char *unidade = "ºC";

        // Realiza a conversão de acordo com a unidade selecionada
        switch (unidade_relatorio)
        {
        case 2: // Kelvin
            temperatura_convertida = celsius_para_kelvin(value);
            max_convertida = celsius_para_kelvin(temp_max);
            min_convertida = celsius_para_kelvin(temp_min);
            unidade = "K";
            break;
        case 3: // Fahrenheit
            temperatura_convertida = celsius_para_fahrenheit(value);
            max_convertida = celsius_para_fahrenheit(temp_max);
            min_convertida = celsius_para_fahrenheit(temp_min);
            unidade = "ºF";
            break;
        case 1: // Celsius (padrão)
        default:
            break;
        }

        float variacao = temperatura_convertida - temperatura_anterior;

        if (temperatura_convertida < min_convertida || temperatura_convertida > max_convertida)
        {
            if (!estava_critica)
            {
                // Caso a temperatura entre em uma zona crítica
                printf("[%s] Temperatura crítica! Temperatura: %.2f%s, ultrapassou os limites. Variação: %.2f%s\n",
                       horario, temperatura_convertida, unidade, variacao, unidade);
                estava_critica = true;
            }
            else
            {
                // Temperatura permanece fora dos limites
                printf("[%s] Temperatura ainda fora do limite. Temperatura: %.2f%s. Variação: %.2f%s\n",
                       horario, temperatura_convertida, unidade, variacao, unidade);
            }
        }
        else
        {
            if (estava_critica)
            {
                // Caso a temperatura tenha voltado ao normal após estar fora dos limites
                printf("[%s] Temperatura estava crítica em %.2f%s, porém estabilizou para %.2f%s\n",
                       horario, temperatura_anterior, unidade, temperatura_convertida, unidade);
                estava_critica = false;
            }
            else
            {
                // Temperatura dentro dos limites
                printf("[%s] Temperatura normal: %.2f%s. Variação: %.2f%s\n",
                       horario, temperatura_convertida, unidade, variacao, unidade);
            }
        }

        // Atualiza a temperatura anterior, já convertida
        temperatura_anterior = temperatura_convertida;
        last_time = now; // Atualiza o tempo da última medição
    }
}

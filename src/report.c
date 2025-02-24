#include <stdio.h>
#include "pico/stdlib.h"
#include "config_uart_functions.h"

// Variáveis globais para controle de temperatura anterior e status
float temperatura_anterior = 0.0;
bool estava_critica = false;
uint64_t last_time = 0;  // Para controle de tempo
int tempo_decorrido = 0;  // Contador de tempo em segundos

// Função para registrar a temperatura com a passagem de tempo
void registrar_temperatura(float temperatura) {
    uint64_t now = to_ms_since_boot(get_absolute_time());  // Tempo atual em milissegundos

    uint64_t delay_ms = tempo_config;
    

    if (now - last_time >= delay_ms) {

        float variacao = temperatura - temperatura_anterior;

        tempo_decorrido++;

        if (temperatura < temp_min || temperatura > temp_max) {
            if (!estava_critica) {
                // Caso a temperatura entre em uma zona crítica
                printf("%lds: Temperatura crítica! Temperatura: %.2f°C, ultrapassou os limites. Aumento: %.2f°C\n",
                       tempo_decorrido, temperatura, variacao);
                estava_critica = true;
            } else {
                // Temperatura permanece fora dos limites
                printf("%lds: Temperatura ainda fora do limite. Temperatura: %.2f°C. Aumento: %.2f°C\n",
                       tempo_decorrido, temperatura, variacao);
            }
        } else {
            if (estava_critica) {
                // Caso a temperatura tenha voltado ao normal após estar fora dos limites
                printf("%lds: Temperatura estava crítica em %.2f°C, porém estabilizou para %.2f°C\n",
                       tempo_decorrido, temperatura_anterior, temperatura);
                estava_critica = false;
            } else {
                // Temperatura dentro dos limites
                printf("%lds: Temperatura normal: %.2f°C. Variação: %.2f°C\n",
                       tempo_decorrido, temperatura, variacao);
            }
        }

        // Atualiza a temperatura anterior
        temperatura_anterior = temperatura;
        last_time = now;  // Atualiza o tempo da última medição
    }

    tight_loop_contents();
}

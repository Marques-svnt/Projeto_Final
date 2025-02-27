// Bibliotecas padrão em C
#include <stdio.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "hardware/pwm.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

// Headers do projeto
#include "defines.h"
#include "display.h"
#include "temperatura.h"

// Definições de constantes
#define INTERVALO_ALARME 1 // Tempo entre mudanças de frequência (ms)
#define FREQ_MIN 310       // Frequência mínima para o buzzer
#define FREQ_MAX 330       // Frequência máxima para o buzzer

// Variáveis globais
volatile bool alarme_ativo = false; // Flag que indica se o alarme está ativo
volatile uint freq_atual = FREQ_MIN;  // Frequência atual do buzzer (inicialmente definida como FREQ_MIN)
struct repeating_timer timer_alarme;  // Estrutura para o timer que controla a alternância da frequência


// Inicializa o buzzer
void buzzer_init()
{
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    pwm_set_enabled(slice_num, false);
}

// Ajusta a frequência do buzzer
void buzz(uint freq)
{
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    uint channel = pwm_gpio_to_channel(BUZZER);

    uint32_t clock_freq = 125000000;
    uint32_t divider = clock_freq / freq / 65536 + 1;
    uint32_t top = clock_freq / (divider * freq);

    pwm_set_clkdiv(slice_num, divider);
    pwm_set_wrap(slice_num, top - 1);
    pwm_set_chan_level(slice_num, channel, top / 2); // Duty cycle 50% (volume máximo)

    pwm_set_enabled(slice_num, true);
}

// Desliga o buzzer
void buzzer_stop()
{
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    uint channel = pwm_gpio_to_channel(BUZZER);

    // Desabilitar PWM
    pwm_set_enabled(slice_num, false);

    // Resetar o duty cycle para 0
    pwm_set_chan_level(slice_num, channel, 0);
}

// Função do timer para alternar a frequência da alarme
bool alternar_alarme(struct repeating_timer *t)
{
    static bool subindo = true;

    if (!alarme_ativo)
    {
        buzzer_stop();
        return false; // Para o timer se o alarme foi desativado
    }

    // Alterna entre FREQ_MIN e FREQ_MAX
    if (subindo)
    {
        freq_atual += 50;
        if (freq_atual >= FREQ_MAX)
            subindo = false;
    }
    else
    {
        freq_atual -= 50;
        if (freq_atual <= FREQ_MIN)
            subindo = true;
    }

    buzz(freq_atual); // Ajusta o buzzer na nova frequência
    return true;      // Mantém o timer rodando
}

// Ativa ou desativa o alarme conforme a temperatura
void alarme_crit(float temp, float temp_min, float temp_max)
{
    if (temp > (temp_max - 0.5) || temp < (temp_min + 0.5))
    {
        if (!alarme_ativo)
        { // Se o alarme ainda não estava ligado
            alarme_ativo = true;
            add_repeating_timer_ms(INTERVALO_ALARME, alternar_alarme, NULL, &timer_alarme);
            display("() INSTAVEL ()", 8, 48);
        }
    }
    else
    {
        alarme_ativo = false;
        uint slice_num = pwm_gpio_to_slice_num(BUZZER);
        uint channel = pwm_gpio_to_channel(BUZZER);

        // Desabilitar PWM
        pwm_set_enabled(slice_num, false);
        display("    ESTAVEL   ", 8, 48);
    }
}

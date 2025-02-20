#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "display.h"
#include "hardware/timer.h"
#include "temperatura.h"
#include "defines.h"

#define INTERVALO_ALARME 1 // Tempo entre mudanças de frequência (ms)
#define FREQ_MIN 310
#define FREQ_MAX 330

#define VOLUME_MAX 100 // Volume máximo (100%)
#define VOLUME_MIN 0   // Volume mínimo (0%)

uint volume = 50; // Volume global (padrão: 50%)

volatile bool alarme_ativo = false;
volatile uint freq_atual = FREQ_MIN;
struct repeating_timer timer_alarme;

// Inicializa o buzzer
void buzzer_init()
{
    gpio_set_function(BUZZER, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER);
    pwm_set_enabled(slice_num, false);
}

// Define o volume (0 a 100%)
void set_volume(uint new_volume)
{
    if (new_volume > VOLUME_MAX)
    {
        volume = VOLUME_MAX;
    }
    else
    {
        volume = new_volume;
    }
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
    pwm_set_chan_level(slice_num, channel, top / 2);

    // Aplica o volume ao duty cycle
    uint32_t duty_cycle = (top / 2) * volume / 100;
    pwm_set_chan_level(slice_num, channel, duty_cycle);

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
            display("INSTAVEL!", 28, 48);
        }
    }
    else
    {
        alarme_ativo = false;
        uint slice_num = pwm_gpio_to_slice_num(BUZZER);
        uint channel = pwm_gpio_to_channel(BUZZER);

        // Desabilitar PWM
        pwm_set_enabled(slice_num, false);
        display(" ESTAVEL ", 28, 48);
    }
}
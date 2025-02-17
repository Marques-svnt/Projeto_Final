#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "display.h"
#include "defines.h"
#include "interrupt.h"
#include "init.h"

extern bool led_estado;

uint16_t lastvrx_value = 2048;
uint16_t lastvry_value = 2048;

uint pwm_init_gpio(uint gpio, uint wrap)
{
    gpio_set_function(gpio, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true);
    return slice_num;
}

void pwm_slice_init()
{
    uint pwm_wrap = 4096;
    // Inicializa PWM para os LEDs
    uint pwm_slice_vermelho = pwm_init_gpio(VERMELHO, pwm_wrap);
    uint pwm_slice_azul = pwm_init_gpio(AZUL, pwm_wrap);
}

void leds_pwm()
{

    if (led_estado)
    {
        // Configura o input do ADC
        adc_select_input(0);
        sleep_us(5); // Pequeno delay para estabilidade
        uint16_t vrx_value = adc_read();
        adc_select_input(1);
        sleep_us(5); // Pequeno delay para estabilidade
        uint16_t vry_value = adc_read();

        // printf("VRX: %u  VRY: %u\n",vrx_value,vry_value); // Debug

        // Cálculos para ligar os leds e configurar a frequência da intensidade
        int i = vrx_value - 2048;
        int j = vry_value - 2048;
        int range = 500;

        // Vê em qual intervalo o joystick se encontra para decidir o estado
        if (i > range) // Range de 2500 até 4095
        {
            pwm_set_gpio_level(VERMELHO, 2 * (vrx_value - 2048));
        }
        else if (i < -range) // Range de 0 até 1500
        {
            pwm_set_gpio_level(VERMELHO, 2 * (-vrx_value + 2048));
        }
        else // Range de 1500 até 2500 (definir o led desligado mesmo com variação de hardware do joystick)
        {
            pwm_set_gpio_level(VERMELHO, 0);
        }

        if (j > range) // Range de 2500 até 4095
        {
            pwm_set_gpio_level(AZUL, 2 * (vry_value - 2048));
        }
        else if (j < -range) // Range de 0 até 1500
        {
            pwm_set_gpio_level(AZUL, 2 * (-vry_value + 2048));
        }
        else // Range de 1500 até 2500 (definir o led desligado mesmo com variação de hardware do joystick)
        {
            pwm_set_gpio_level(AZUL, 0);
        }
    }
    // Não liga nada se o botão do joystick estiver dando o retorno falso
    else
    {
        pwm_set_gpio_level(VERMELHO, 0);
        pwm_set_gpio_level(AZUL, 0);
    }
}

void movimentoJoystick()
{
    // Mapeando valores do ADC para o display
    adc_select_input(0);
    sleep_us(5); // Pequeno delay para estabilidade
    uint16_t vrx_value = adc_read();
    adc_select_input(1);
    sleep_us(5); // Pequeno delay para estabilidade
    uint16_t vry_value = adc_read();

    // Variável que faz o controle para que haja atualização apenas quando o joystick se mover evitando que ele fique tremendo o tempo todo
    int flick_x = vrx_value - lastvrx_value;
    int flick_y = vry_value - lastvry_value;

    if (!(flick_x < 100 && flick_x > -100 && flick_y < 100 && flick_y > -100)) // Apenas ocorre o código quando o valor de flick fugir do intervalo entre -100 e 100
    {
        int coord_x = (vry_value * 126) / 4095; // Mapeando para intervalo de 0-127
        int coord_y = (vrx_value * 40) / 4095;  // Mapeando para intervalo de 0-63

        lastvry_value = vry_value;
        lastvrx_value = vrx_value;

        // Definir limites para coordenadas
        int min_x = 10, max_x = 112;
        int min_y = 10, max_y = 48;

        // Inversão do eixo Y devido ao display exibir os movimentos ao contrário
        coord_y = max_y - coord_y;

        // Garantir que as coordenadas fiquem dentro dos limites
        if (coord_x < min_x)
        {
            coord_x = min_x;
        }
        if (coord_x > max_x)
        {
            coord_x = max_x;
        }
        if (coord_y < min_y)
        {
            coord_y = min_y;
        }
        if (coord_y > max_y)
        {
            coord_y = max_y;
        }

        // Exibir as coordenadas
        // printf("X: %d, Y: %d\n", coord_x, coord_y); // Debug
        display(coord_x, coord_y);

        // Delay de estabilização
        sleep_us(5);
    }
}

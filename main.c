#include <stdio.h>
#include "pico/stdlib.h"
#include "defines.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "display.h"
#include "init.h"
#include "interrupt.h"
#include "pwm.h"

int main()
{

    // Função que chama todas as inicializações do projeto
    init();

    // Rotinas de interrupção para os botões A e joystick pressionado
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Loop principal
    while (true)
    {
        leds_pwm();  // Chama a configuração dos leds pwm
        movimentoJoystick(); // Chama a configuração do movimento sincronizado com o joystick
        sleep_ms(50);
    }
}

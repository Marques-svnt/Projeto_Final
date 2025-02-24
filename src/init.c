#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "interrupt.h"
#include "display.h"
#include "pwm.h"
#include "buzzer_temp.h"
#include "pio.h"
#include "defines.h"

extern int choose;

// Inicializa o joystick
void joystick_init()
{
    // Configura o ADC do joystick e o estado do botão
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);
}

// Configura o botão A e os leds
void led_button_init()
{

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(VERDE);
    gpio_set_dir(VERDE, GPIO_OUT);

    gpio_init(VERMELHO);
    gpio_set_dir(VERMELHO, GPIO_OUT);

    gpio_init(AZUL);
    gpio_set_dir(AZUL, GPIO_OUT);
}

// Função que vai configurar os botões do menu
void menu_init(){
    limpar();

    // Exibição inicial
    set_one_led(1, 0, 0, 20);
    display("Sensor", 40, 15);
    display("Temperatura", 20, 25);
    display(" <     B     >",4,45);

    choose = 1;

    // Configuração da interrupção com callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

// Faz todas as inicializações do projeto
void init()
{
    stdio_init_all();
    initI2C();
    uart_init(uart0,115200);
    joystick_init();
    led_button_init();
    pwm_slice_init();
    initializePio();
    buzzer_init();
}
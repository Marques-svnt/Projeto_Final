// Bibliotecas padrão em C
#include <stdio.h>

// Bibliotecas de hardware do Raspberry Pi Pico
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"

// Headers do projeto
#include "buzzer_temp.h"
#include "defines.h"
#include "display.h"
#include "interrupt.h"
#include "pio.h"
#include "pwm.h"

// Variáveis externas
extern int choose;

// Inicializa o joystick (configura o ADC e o pino do joystick)
void joystick_init() {
    adc_init(); // Inicializa o ADC
    adc_gpio_init(JOYSTICK_Y_PIN); // Inicializa o pino Y do joystick
}

// Configura os botões A, B e os LEDs
void led_button_init() {
    
    // Configuração dos botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN); 
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN); 
    gpio_pull_up(BUTTON_B); 

    // Configuração dos LEDs
    gpio_init(VERDE);
    gpio_set_dir(VERDE, GPIO_OUT); 

    gpio_init(VERMELHO);
    gpio_set_dir(VERMELHO, GPIO_OUT); 
}

// Função para configurar o menu, incluindo a exibição e interrupções de botões
void menu_init() {
    limpar(); // Limpa o display

    // Exibição inicial
    set_one_led(1, 0, 0, 20);
    display("Sensor", 40, 15);
    display("Temperatura", 20, 25); 
    display(" <     B     >", 4, 45);

    choose = 1; // Define a escolha inicial

    // Configuração da interrupção dos botões A e B com callback
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // Interrupção no botão A

    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler); // Interrupção no botão B
}

// Função que faz todas as inicializações do projeto
void init() {
    stdio_init_all(); // Inicializa a comunicação serial

    initI2C(); // Inicializa o barramento I2C

    uart_init(uart0, 115200); // Inicializa o UART com baud rate de 115200

    joystick_init(); // Inicializa o joystick

    led_button_init(); // Inicializa os botões e LEDs

    pwm_slice_init(); // Inicializa o PWM

    initializePio(); // Inicializa a PIO

    buzzer_init(); // Inicializa o buzzer
}

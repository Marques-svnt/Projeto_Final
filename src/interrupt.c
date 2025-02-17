#include <stdio.h>
#include "pico/stdlib.h"
#include "defines.h"
#include "display.h"
#include "pwm.h"

// Variáveis globais
static volatile uint a = 1;
static volatile uint j = 1;
static volatile uint32_t last_time_A = 0; // Armazena o tempo do último evento (em microssegundos)
static volatile uint32_t last_time_J = 0;
bool led_estado = true;
bool rect_estado = true;
bool estado_verde = false;

// Função responsável pelo debounce
bool debounce(volatile uint32_t *last_time, uint32_t debounce_time) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > debounce_time) {
        *last_time = current_time;
        return true;
    }
    return false;
}

// Função de interrupção com debouncing
void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    // Desligar ou ligar os leds de ligarem ao apertar o joystick
    if (gpio == BUTTON_A && debounce(&last_time_A, 200000))
    {
        last_time_A = current_time;
        printf("Botão A pressionado: %d vezes\n", a); // Para controle quando se usa o monitor serial para verificar se há bouncing
        a++;
        led_estado = !led_estado;
        
        if (led_estado == true){
            printf("Ligando os leds PWM\n");
        } else {
            printf("Desligando os leds PWM\n");
        }

    } else if (gpio == JOYSTICK_PB && debounce(&last_time_J, 200000))
    {
        last_time_J = current_time;
        printf("Joystick pressionado: %d vezes\nAlterando a borda do retângulo", j); // Para controle quando se usa o monitor serial para verificar se há bouncing
        j++;
        rect_estado = !rect_estado; // Muda o estado do retângulo para alterar o estilo

        // Liga ou desliga o led verde
        estado_verde = !estado_verde; 
        gpio_put(VERDE, estado_verde);

        if(estado_verde == true){
            printf(" e ligando o led verde\n");
        } else {
            printf(" e desligando o led verde\n");
        }
    }
}

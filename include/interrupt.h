#ifndef INTERRUPT_H
#define INTERRUPT_H

//Declaração da função de interrupção
void gpio_irq_handler(uint gpio, uint32_t events);
bool debounce(volatile uint32_t *last_time, uint32_t debounce_time);

#endif
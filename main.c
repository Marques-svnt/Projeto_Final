#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "defines.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"
#include "display.h"
#include "init.h"
#include "interrupt.h"
#include "pwm.h"

int main()
{

    // Função que chama todas as inicializações do projeto
    init();

    // Loop principal
    while (true)
    {
        sleep_ms(50);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "init.h"

int config() {
    if (stdio_usb_connected()) {
        // USB is connected
        printf("USB is connected.\n");
    } else {
        limpar();
        display("Ative o Serial",8,10);
        display("Monitor",36,22);
        display("e tente",36,34);
        display("novamente",28,46);
        sleep_ms(4000);
    }
    menu_init(); // configura novamente os botões do menu
    return 0;
}
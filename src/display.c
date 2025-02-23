#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "defines.h"

bool cor = true;
ssd1306_t ssd;

extern int clean;

void initI2C()
{
    // Inicializa o I2C e configura como 400kHz
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Função que recebe o texto e coordenadas para exibir no display a mensagem
void display(const char *texto, int x, int y)
{
    // Atualiza o conteúdo do display com animações
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_rect(&ssd, 3, 3, 123, 59, cor, !cor);
    ssd1306_draw_string(&ssd, texto, x, y);       // Desenha a string passada como argumento
    ssd1306_send_data(&ssd);                      // Atualiza o display
}

void limpar()
{
    ssd1306_fill(&ssd, false);
}

void display_set_temp(const char *texto, int x, int y)
{
    // Atualiza o conteúdo do display com animações
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
    ssd1306_rect(&ssd, 3, 3, 123, 59, cor, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 40, cor, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 39, cor, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 41, cor, !cor);
    ssd1306_draw_string(&ssd, texto, x, y);       // Desenha a string passada como argumento
    ssd1306_send_data(&ssd);                      // Atualiza o display
}
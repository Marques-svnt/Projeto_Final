#ifndef CONFIG_UART_FUNCTIONS_H
#define CONFIG_UART_FUNCTIONS_H

#include <stdbool.h> // Adicionar função booleana

extern const float temp_min_fabrica;
extern const float temp_max_fabrica;
extern const float incremento_fabrica;
extern const int tempo_config_fabrica;
extern const int unidade_relatorio_fabrica;

extern volatile float temp_min;
extern volatile float temp_max;
extern volatile float incremento;

extern volatile float temp_min_temporario;
extern volatile float temp_max_temporario;
extern volatile float incremento_temporario;

extern volatile float temp_crit_min;
extern volatile float temp_crit_max;

extern volatile int tempo_config;

extern volatile int unidade_relatorio;

//Declaração das funções de configuração via UART
void save_and_quit();
void no_save();
int alterar_valor(int alterar);
void reset_config_fabrica();
bool verificar_config_fabrica();
void menu_off();

#endif
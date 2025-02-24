#ifndef CONFIG_UART_FUNCTIONS_H
#define CONFIG_UART_FUNCTIONS_H

#include <stdbool.h> // Adicionar função booleana

// Variaveis relacioandas à temperatura
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

//Declaração das funções de configuração da temperatura via UART
void save_and_quit_temp();
void no_save_temp();
int alterar_valor(int alterar);
void reset_config_fabrica();
bool verificar_config_fabrica();
void menu_off();

// Variáveis reçacioanadas ao relatório
extern volatile int tempo_config;
extern volatile int unidade_relatorio;
extern bool gerar_relatorio; // 1 para ligado e 0 para desligado

//Declaração das funções de configuração do relatório via UART
void mudar_gerar_relatorio();
int mudar_unidade_relatorio();
int mudar_intervalo_relatório();
void no_save_relatorio();
void save_and_quit_relatorio();

#endif
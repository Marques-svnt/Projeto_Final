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

// Declaração das funções de configuração gerais
void show_config();
void usb_off();
void reset_config_fabrica();
bool verificar_config_fabrica();
int ler_inteiro_seguro();
float ler_float_seguro();

//Declaração das funções de configuração da temperatura via UART
void save_and_quit_temp();
void no_save_temp();
int alterar_valor(int alterar);

// Variáveis reçacioanadas ao relatório
extern volatile int tempo_config;
extern volatile int unidade_relatorio;
extern volatile int unidade_relatorio_temporario;
extern volatile int tempo_config_temporario;
extern bool gerar_relatorio; // 1 para ligado e 0 para desligado

//Declaração das funções de configuração do relatório via UART
void mudar_gerar_relatorio();
int mudar_unidade_relatorio();
int mudar_intervalo_relatorio();
void no_save_relatorio();
void save_and_quit_relatorio();

#endif
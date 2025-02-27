#ifndef BUZZER_TEMP_H
#define BUZZER_TEMP_H

//Declaração das funções do buzzer
void buzz();
void buzzer_stop();
void buzzer_init();
void alarme_crit(float temp, float temp_min, float temp_max);

#endif
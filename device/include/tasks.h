#ifndef TASKS_H 
#define TASKS_H

#include "wifiModule.h"
#include "oled.h"
#include "buttons.h"

void vTaskUI(void * param);
void vTaskOled(void * param);

void vTaskHX711_A(void * param);
void vTaskHX711_B(void * param);
void vTaskHX711_C(void * param);
void vTaskHX711_D(void * param);

void vTaskButtons(void * param);


#endif
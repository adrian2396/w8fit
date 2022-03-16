#include "tasks.h"
#include "hx711.h"

extern pthread_mutex_t mutex;

extern byte DOUT[CELL_NUMBER];
extern byte CLK[CELL_NUMBER];

extern int state;
extern int next;

extern float w1;
extern float w2;
extern float w3;
extern float w4;

void vTaskUI(void * param){
    wifiModule xWeb = *(wifiModule*)param;

    Serial.println("");
    Serial.println("[tasks.cpp]: UI Task running on core");
    Serial.println(xPortGetCoreID());

    /* INICIALIZE WEB */
    Serial.println("[tasks.cpp]: Inicialize Web");
    xWeb.vSelectWifiMode();

    while(1) { 
        pthread_mutex_lock(&mutex);
        if (xWeb.runMode == SOFT_AP_MODE){   
            xWeb.softAP->server->handleClient();
            vTaskDelay(1); 
        }
        else if (xWeb.runMode == STATION_MODE) {
            
            vTaskDelay(1);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void vTaskOled(void * param){
    oled xOled = *(oled*)param;

    Serial.println("");
    Serial.println("[tasks.cpp]: Oled Task running on core");
    Serial.println(xPortGetCoreID());

    Wire.begin(SDA, SCL,100000);

    Serial.println("[tasks.cpp]: OLED init ");
    xOled.vOledBegin();

    Serial.println("[tasks.cpp]: display init ");
    xOled.display->display();
    delay(2000); // Pause for 2 seconds

    while (1){
        pthread_mutex_lock(&mutex);
        vTaskDelay(100);
        pthread_mutex_unlock(&mutex);
    }
    
}

void vTaskHX711_A(void * param){
    HX711 HX711_A = *(HX711*)param;

    Serial.println("");
    Serial.print("[tasks.cpp]: HX711_A Task running on core ");
    Serial.println(xPortGetCoreID());

    while (1){
        pthread_mutex_lock(&mutex);
        HX711_A.vCheckStateA(DOUT[CELL_A], CLK[CELL_A]);
        pthread_mutex_unlock(&mutex);
    }
}
void vTaskHX711_B(void * param){
    HX711 HX711_B = *(HX711*)param;

    Serial.println("");
    Serial.print("[tasks.cpp]: HX711_B Task running on core ");
    Serial.println(xPortGetCoreID());

    while (1){
        pthread_mutex_lock(&mutex);
        HX711_B.vCheckStateB(DOUT[CELL_B], CLK[CELL_B]);
        pthread_mutex_unlock(&mutex);
    }
}
void vTaskHX711_C(void * param){
    HX711 HX711_C = *(HX711*)param;

    Serial.println("");
    Serial.print("[tasks.cpp]: HX711_C Task running on core ");
    Serial.println(xPortGetCoreID());

    while (1){  
        pthread_mutex_lock(&mutex);
        HX711_C.vCheckStateC(DOUT[CELL_C], CLK[CELL_C]);
        pthread_mutex_unlock(&mutex);
    }
}
void vTaskHX711_D(void * param){
    HX711 HX711_D = *(HX711*)param;

    Serial.println("");
    Serial.print("[tasks.cpp]: HX711_D Task running on core ");
    Serial.println(xPortGetCoreID());

    while (1){
        pthread_mutex_lock(&mutex);
        HX711_D.vCheckStateD(DOUT[CELL_D], CLK[CELL_D]);
        pthread_mutex_unlock(&mutex);
    }
}

void vTaskButtons(void * param){
    buttons buttons_ = *(buttons*)param;

    vTaskDelay(1000);

    Serial.println("");
    Serial.print("[tasks.cpp]: Buttons Task running on core ");
    Serial.println(xPortGetCoreID());

    while (1){
        buttons_.vActualizeOutputs();
        vTaskDelay(100);
    }
}



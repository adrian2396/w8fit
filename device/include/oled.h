#ifndef OLED_H 
#define OLED_H

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "parameters.h"

class oled{
private:
    /* data */
public:
    oled(Adafruit_SSD1306 *display_);
    ~oled();

    Adafruit_SSD1306 *display;

    void vOledBegin();
    void vDrawBalance();
    void vDrawWeigth(int weigth, int i, int j );
    void vSelectCell(int w1, int w2, int w3, int w4,int cell_number);
    void vSendData();
    void vInitCalibration();
    void vCellCalibration(int cell_number);
    void vCalibrated();

};




#endif
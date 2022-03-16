#ifndef BUTTONS_h
#define BUTTONS_h

#include <Arduino.h>

#include "parameters.h"

#define SELECT     14 
#define CALIBRATE   27
#define GREEN   19
#define RED 18

class buttons{
private:
	/* data */
public:
	buttons(/* args */);
	~buttons();

    void vCheckSelect();
    void vCheckCalibrate();
    void vActualizeOutputs();

    void vCheckStateSelect();
    void vCheckStateCalibrate();
};


#endif
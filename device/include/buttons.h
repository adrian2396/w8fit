#ifndef BUTTONS_h
#define BUTTONS_h

#include <Arduino.h>

#include "parameters.h"


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

    void vPrintState();
};


#endif
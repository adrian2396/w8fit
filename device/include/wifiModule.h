#ifndef WIFIMODULE_H 
#define WIFIMODULE_H

#include "softAP.h"
#include "station.h"
#include "parameters.h"


class wifiModule{
private:

public:
    wifiModule(softAPMode *softAP_, stationMode * station_);
    ~wifiModule();

    softAPMode *softAP;
    stationMode *station;

    int runMode = SOFT_AP_MODE;

    void vSelectWifiMode();
};

#endif
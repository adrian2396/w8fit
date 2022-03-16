#ifndef STATION_H 
#define STATION_H

#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

#include "parameters.h"


class stationMode{
private:
    static void vAsyncHandleNotFound(AsyncWebServerRequest *request);
public:
    stationMode();
    ~stationMode();
    
    AsyncWebServer *asyncServer;

    void runAsStation();
};

#endif
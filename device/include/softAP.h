#ifndef SOFTAP_H 
#define SOFTAP_H

#include <WiFi.h>
#include <WebServer.h>

#include "eprom.h"
#include "parameters.h"
#include "debug.h"

class softAPMode{
private:
    const char* SSID_PREFIX = "WeightFit-";        // SSID prefix as access point
    const char* PASSWORD    = "12345678";                 // Password as access point

    static void vFrontendWifiCredentials();
    void vGetWifiCredentials();
public:
    softAPMode();
    ~softAPMode();

    WebServer *server; 

    String  username_sta = "";
    String  password_sta = "";

    char username_sta_array[33];                // SSID as a char array
    char password_sta_array[33];                // Password as a char array

    uint8_t mac[6];

    void runAsAPServer(uint8_t mac[]);
    static void vHandleNotFound();
    
};

#endif
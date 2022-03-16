#include "wifiModule.h"

wifiModule *this_;

// ------------------- WIFIMODULE CLASS METHODS ------------------//
wifiModule::wifiModule(softAPMode *softAP_, stationMode * station_){
    softAP = softAP_;
    station = station_;
    this_ = this;
    
}

wifiModule::~wifiModule(){
}

void wifiModule::vSelectWifiMode(){
    softAP->username_sta = eeprom_read(EEPROM_SSID_STA_KEY);
    softAP->password_sta = eeprom_read(EEPROM_PASWD_STA_KEY);

    Serial.println(softAP->username_sta);
    Serial.println(softAP->password_sta);

    if (softAP->username_sta.length() > 1 && softAP->password_sta.length() > 1){

        softAP->username_sta.toCharArray(softAP->username_sta_array, sizeof(softAP->username_sta_array));
        softAP->password_sta.toCharArray(softAP->password_sta_array, sizeof(softAP->password_sta_array));

        Serial.println("[webHandle.cpp]: Got connection parameters");
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(softAP->username_sta_array, softAP->password_sta_array);

        int retries = 20;

        while (WiFi.status() != WL_CONNECTED && retries > 0) {
            Serial.println("[webHandle.cpp]: Connecting to WiFi..");
            retries --;
            vTaskDelay(500);
        }
        if(WiFi.status() != WL_CONNECTED) {
            Serial.println("[webHandle.cpp]: Invalid credentials, restarting...");
            eeprom_write(EEPROM_SSID_STA_KEY, "");
            eeprom_write(EEPROM_PASWD_STA_KEY, "");
            eeprom_commit();
            ESP.restart();
        }
        else {
          DEBUG_MSG("\n[webHandle.cpp]: WiFi connected to IP address: %s\n", WiFi.localIP().toString().c_str());
          Serial.println("[webHandle.cpp]: Connected to the WiFi network");

          runMode = STATION_MODE;
          station->runAsStation();
        }
    } 
    else {  
        Serial.println("[webHandle.cpp]: Server As a AP");
        WiFi.softAPmacAddress(softAP->mac);
        softAP->runAsAPServer(softAP->mac);
        runMode = SOFT_AP_MODE;
        vTaskDelay(5);
    }    
}

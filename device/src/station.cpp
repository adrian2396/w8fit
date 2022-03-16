#include "station.h"

//pthread_mutex_t mutexStation = PTHREAD_MUTEX_INITIALIZER;

// --------------------- STATION CLASS METHODS ---------------------//
stationMode::stationMode(){
    asyncServer = new AsyncWebServer(80);
    //pthread_mutex_init(&mutexStation, 0);
}
stationMode::~stationMode(){
}


void stationMode::runAsStation(){
    Serial.println("[station.cpp]: web server");
    asyncServer->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
      Serial.println("[oledHandle]: serving frontend");
      AsyncWebServerResponse *response; 
      response = request->beginResponse_P(200, "text/html", "");
      response->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      response->addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
      response->addHeader("Access-Control-Allow-Origin","*");
      request->send(response);
      Serial.println("[station.cpp]: frontend served");
    });
    asyncServer->onRequestBody([this] (AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
      Serial.println("[station.cpp]: body request");
      AsyncWebServerResponse *response = NULL; 
      if (request->method() == HTTP_OPTIONS) {
        response = request->beginResponse(200);
      }
      if (request->method() == HTTP_POST) {
        Serial.println("[station.cpp]: POST requets");
        response = request->beginResponse(201);
        const char* body = (const char*) data;
        String path = request->url();
  
        // >>> 
        if (path.startsWith("/row/")) {
          StaticJsonDocument<256> nCard;
          DeserializationError error = deserializeJson(nCard, body);

          if (error) {
            Serial.print(F("[station.cpp]: DeserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
          }
        }
      }
      response->addHeader("Access-Control-Allow-Methods", "GET, POST");
      response->addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
      response->addHeader("Access-Control-Allow-Origin","*");
      request->send(response);   
    });

    asyncServer->onNotFound(vAsyncHandleNotFound);
    asyncServer->begin();
    Serial.printf("[webHandle.cpp]: Server as a Run Station\n");
}


void stationMode::vAsyncHandleNotFound(AsyncWebServerRequest *request){
  //Handle Unknown Request
  request->send(404);
}
#include "softAP.h"

softAPMode *this__;
// --------------------- SOFTAP CLASS METHODS ---------------------//
softAPMode::softAPMode(){
    server = new WebServer(80);
    this__ = this;
}
softAPMode::~softAPMode(){
}

void softAPMode::runAsAPServer(uint8_t mac[]){
  char ssid[33];        // SSID. Maximum length is 32 characters
  char ip_address[16];  // IPV4 address as a char array

  String s_ssid;
  String s_ip;
  String s_password;

  WiFi.softAPIP().toString().toCharArray(ip_address, sizeof(ip_address));
  
  // Generate the SSID from the MAC address
  sprintf(ssid, "%s%02X%02X", SSID_PREFIX, mac[4], mac[5]);
  
  // Set the access point
  Serial.printf("[sofotAP.cpp]: Setting AP with SSID = %s\n", ssid);
  Serial.printf(WiFi.softAP(ssid, PASSWORD) ? "[webHandle.cpp]: OK\n" : " [sofotAP.cpp]: Failed\n");
  Serial.printf("[sofotAP.cpp]: IP address: %s\n", ip_address);
  
  // Oled diplay (show ssid, password and ip adress)

  // Start the web server
  server->on("/", vFrontendWifiCredentials);
  server->onNotFound(vHandleNotFound);
  server->begin();
  Serial.printf("[sofotAP.cpp]: Web server started\n");

  // Show display
  s_ssid = String (ssid);
  s_ip = String (ip_address);
  s_password = String(s_password);
}

void softAPMode::vFrontendWifiCredentials(){
    String frontendWifi = "<html lang=' '>\n\
  <head>\n\
    <meta charset='utf-8'>\n\
    <meta\n\
     name='viewport'\n\
     content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0'\n\
     />\n\
    <title>W8Fit</title>\n\
    <!--link rel='stylesheet' href='styles.css'-->\n\
    <style>\n\
      header {background: #2C3E50;color: #FFF;}\n\
      .main {background: #F2F2F2;}\n\
      .container {width: 100%;margin-right: auto;margin-left: auto;}\n\
      .disclaimer {width: 100%; margin-left: 610px;margin-right: 610px;}\n\
      .form-container {height: 20px;width: 100%;margin-right: auto;margin-left: auto;margin-bottom: 50px;margin-top: 40px;position: relative;}\n\
      .text {background: #F2F2F2; margin-right: 1220px;}\n\
      .form {background: #F2F2F2;}\n\
      .form-control {display: block;width: 300px;height: calc(2.25rem + 2px);padding: .375rem .75rem;font-size: 1rem;\n\
        line-height: 1.5;color: #495057;background-color: #fff;background-clip: padding-box;border: 1px solid #ced4da;\n\
        border-radius: .25rem;transition: border-color .15s ease-in-out,box-shadow .15s ease-in-out;margin: 0;position: absolute;\n\
        top: 50%;left: 50%;-ms-transform: translate(-50%, -50%);transform: translate(-50%, -50%);}\n\
      .button-back {margin-top: 80px;}\n\
      .button {background-color: #2C3E50;padding: 7px 54px;border-radius: 8px;font-size: 20px;color: white;\n\
        position: absolute;left: 50%;-ms-transform: translate(-50%, -50%);transform: translate(-50%, -50%);}\n\
      .aside {background: #C0392B;color: #FFF;}\n\
      .checkbox-container {width: 300px;margin: 0;position: absolute;left: 50%;bottom: -60px;-ms-transform: translate(-50%, -50%);transform: translate(-50%, -50%);}\n\
      footer {background: #16A085;color: #FFF;}\n\
    </style>\n\
  </head>\n\
  <body onload='window.scrollTo(0.5)'>\n\
    <header>\n\
      <div class='container'>\n\
        <h1 style='text-align: center;'>W8Fit</h1>\n\
      </div>\n\
    </header>\n\
    <div class='disclaimer'>\n\
      <section class='text'>\n\
        <div class='description'>\n\
          <p style=''>\n\
             Provide a wifi acces point with your mobile phone.<br>\n\
             Enter your wifi credentials.<br>\n\
             Then go to: 192.168.0.24 .<br>\n\
        </div>\n\
      </section>\n\
    </div>\n\
    <div class='container'>\n\
      <section>\n\
        <form action='' method='post'>\n\
          <div class='form-container'>\n\
            <input class='form-control' value='' title='' name='username' type='text' placeholder='Red Wifi'\n\
              onfocus='if(this.value==this.title) {this.value=\"\";}'\n\
              onblur='if(this.value==\"\") {this.value=this.title;}'>\n\
          </div>\n\
          <div class='form-container'>\n\
            <input class='form-control' value='' title='' id='password' name='password' type='password' placeholder='Password'\n\
              onfocus='if(this.value==this.title) {this.value=\"\";}'\n\
              onblur='if(this.value==\"\") {this.value=this.title;}'>\n\
            <div class='checkbox-container'>\n\
              <input type='checkbox' onclick='let x = document.getElementById(\"password\");if(x.type === \"password\"){x.type = \"text\";}else{x.type = \"password\";}'>Show Password\n\
            </div>\n\
          </div>\n\
          <div class='container button-back'>\n\
            <button class='button' onclick='document.getElementById(\"formulariocredenciales\").submit();'>Submit</button>\n\
          </div>\n\
        </form>\n\
      </section>\n\
    </div>\n\
    <div class='footer'>\n\
    </div>\n\
    </body>\n\
    </html>";
    this__->server->send(200, "text/html", frontendWifi);
    this__->vGetWifiCredentials();
}

void softAPMode::vGetWifiCredentials(){
  if (server->args() > 0){
    for (int i = 0; i < server->args(); i++) {
        if (server->argName(i).equals("username"))
            username_sta = server->arg(i);
        else if (server->argName(i).equals("password"))
            password_sta = server->arg(i);
    }
    if (!username_sta.equals("username") && !password_sta.equals("password")){
        // Store SSID and password in EEPROM
        eeprom_write(EEPROM_SSID_STA_KEY, username_sta);
        eeprom_write(EEPROM_PASWD_STA_KEY, password_sta);
        eeprom_commit();
        username_sta.toCharArray(username_sta_array, sizeof(username_sta_array));
        password_sta.toCharArray(password_sta_array, sizeof(password_sta_array));
        DEBUG_MSG("Station SSID: %s\n", username_sta_array);
        DEBUG_MSG("Station passwd: %s\n", password_sta_array);
        DEBUG_MSG("Storing in EEPROM\n");
        ESP.restart();
    }
  }
}

void softAPMode::vHandleNotFound() {
    //this_->server->send(404, "text/plain", this_->message);
}
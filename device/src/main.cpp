#include <Arduino.h>
#include <string.h>
#include <WebServer.h>
#include <WiFi.h>

#include "tasks.h"
#include "hx711.h"

#define TASK_STACK_SIZE             10000
#define TASK_PRIORITY               0

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin
#define SDA   21
#define SCL   22

// Set Set static IP address
IPAddress local_IP(192,168,1,131); 
IPAddress gateway(192,168,1,131);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);     //optional
IPAddress secondaryDNS(8, 8, 4, 4);   //optional

// Class
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


buttons buttons_;
oled oled_(&display);
softAPMode softAP;
stationMode station;
wifiModule wifiMode(&softAP, &station);

HX711 hx711_A;
HX711 hx711_B;
HX711 hx711_C;
HX711 hx711_D;

float w1 = 0;
float w2 = 0;
float w3 = 0;
float w4 = 0; 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

byte DOUT[CELL_NUMBER + 1] = {0, 34, 12, 4, 33};
byte CLK[CELL_NUMBER + 1] = {0,32, 13, 0, 25};

// variables
int state = ACTIVE;
int next = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\n");

  /* configures static ip adress */
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  pthread_mutex_init(&mutex, 0);


  Wire.begin(SDA, SCL, 100000);

  oled_.vOledBegin();

  oled_.vDrawBalance();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, WHITE);

  //eeprom_write(EEPROM_SSID_STA_KEY, "");
  //eeprom_write(EEPROM_PASWD_STA_KEY, "");
  //eeprom_write(EEPROM_STEPPER, "");

  // HX711
  DOUT[0] = 34;
  DOUT[1] = 12;
  DOUT[2] = 26;
  DOUT[3] = 33;

  CLK[0] = 32;
  CLK[1] = 13;
  CLK[2] = 4;
  CLK[3] = 25;

  /*pinMode(DOUT[0], INPUT);
  pinMode(DOUT[1], INPUT);
  pinMode(DOUT[2], INPUT);
  pinMode(DOUT[3], INPUT);

  pinMode(CLK[0], INPUT);
  pinMode(CLK[1], INPUT);
  pinMode(CLK[2], INPUT);
  pinMode(CLK[3], INPUT);*/

  pinMode(CALIBRATE, INPUT);
  pinMode(SELECT, INPUT);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);


  hx711_A.vSetUp(DOUT[0], CLK[0]);
  hx711_B.vSetUp(DOUT[1], CLK[1]);
  //hx711_C.vSetUp(DOUT[2], CLK[2]);
  hx711_D.vSetUp(DOUT[3], CLK[3]);


  /* TASKS */
  /*TaskHandle_t xHandleUI = NULL;
  Serial.println("[main.cpp]: UI task created");
  xTaskCreatePinnedToCore(vTaskUI, "UI Task", TASK_STACK_SIZE, (void*)&wifiMode, TASK_PRIORITY + 3, &xHandleUI, 1);
  configASSERT(xHandleUI);

  TaskHandle_t xHandleOled = NULL;
  Serial.println("[main.cpp]: OLED task created");
  xTaskCreatePinnedToCore(vTaskOled, "OLED Task", TASK_STACK_SIZE, (void*)&oled_, TASK_PRIORITY + 1, &xHandleOled, 0);
  configASSERT(xHandleOled);*/

  TaskHandle_t xHandleHX711_A;
  Serial.println("[main.cpp]: HX711_A Task created");
  xTaskCreatePinnedToCore(vTaskHX711_A, "HX711_A Task", TASK_STACK_SIZE, (void*)&hx711_A, TASK_PRIORITY + 1, &xHandleHX711_A, 1);
  configASSERT(xHandleHX711_A);

  TaskHandle_t xHandleHX711_B;
  Serial.println("[main.cpp]: HX711_B Task created");
  xTaskCreatePinnedToCore(vTaskHX711_B, "HX711_B Task", TASK_STACK_SIZE, (void*)&hx711_B, TASK_PRIORITY + 1, &xHandleHX711_B, 1);
  configASSERT(xHandleHX711_B);

  TaskHandle_t xHandleHX711_C;
  Serial.println("[main.cpp]: HX711_C Task created");
  xTaskCreatePinnedToCore(vTaskHX711_C, "HX711_C Task", TASK_STACK_SIZE, (void*)&hx711_C, TASK_PRIORITY + 1, &xHandleHX711_C, 1);
  configASSERT(xHandleHX711_C);

  TaskHandle_t xHandleHX711_D;
  Serial.println("[main.cpp]: HX711_D Task created");
  xTaskCreatePinnedToCore(vTaskHX711_D, "HX711_D Task", TASK_STACK_SIZE, (void*)&hx711_D, TASK_PRIORITY + 1, &xHandleHX711_D, 1);
  configASSERT(xHandleHX711_D);

  TaskHandle_t xHandleButtons;
  Serial.println("[main.cpp]: Button Task created");
  xTaskCreatePinnedToCore(vTaskButtons, "Buttons Task", TASK_STACK_SIZE, (void*)&buttons_, TASK_PRIORITY + 1, &xHandleButtons, 0);
  configASSERT(xHandleButtons);

  //vTaskSuspend(NULL);
}

void loop() {
  /*byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();

      Wire.beginTransmission(address+1);

  if (error == 0 && Wire.endTransmission() != 0 ) // Special flag for SAMD Series
  {
      Serial.print("I2C device found at address 0x");
      if (address<16)
          Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("!");

      nDevices++;
  }
  else if (error==4) 
  {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
          Serial.print("0");
      Serial.println(address,HEX);
  }
  }
  if (nDevices == 0)
      Serial.println("No I2C devices found\n");
  else
      Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan*/

  return;
}
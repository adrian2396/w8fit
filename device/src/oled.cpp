#include "oled.h"

extern float w1;
extern float w2;
extern float w3;
extern float w4;

extern int selectWeight;
extern int calibrateWeight;
extern float knownWeight;

extern int state;
extern int previousState;
extern int next;

extern int actualice;

oled::oled(Adafruit_SSD1306 *display_)
{
    display = display_;
}

oled::~oled()
{
}


void oled::vOledBegin()
{
    if(!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

void oled::vDrawBalance() 
{
    display->clearDisplay();

    display->fillRoundRect(5,0,117,13,3,WHITE);
    display->setCursor(50,3);
    display->setTextColor(BLACK);
    display->setTextSize(1);
    display->print("W8FIT");
    display->drawRoundRect(5,16,117,47,10,WHITE);
    display->drawFastVLine(12,16,47,WHITE);
    display->drawFastVLine(114,16,47,WHITE);
    display->fillRoundRect(20,23,40,13,1,WHITE);
    display->fillRoundRect(67,23,40,13,1,WHITE);
    display->fillRoundRect(20,43,40,13,1,WHITE);
    display->fillRoundRect(67,43,40,13,1,WHITE);

    display->display();
}

void oled::vDrawWeigth(int weigth, int i, int j)
{

    int weightPosition;

    int x[2][3] = {{28, 25, 22}, {75, 72, 69}};
    int y[2] = {26, 46};

    if (weigth >= 0 && weigth <10) weightPosition = 0;
    if (weigth >= 10 && weigth <100) weightPosition = 1;
    if (weigth >= 100) weightPosition = 2;

    display->setCursor(x[i][weightPosition], y[j]);
    display->setTextColor(BLACK);
    display->setTextSize(1);
    display->print(weigth);
    display->println(" gr");

    display->display();
}


void oled::vSendData()
{
    display->clearDisplay();

    display->setCursor(25,16);
    display->setTextColor(WHITE);
    display->setTextSize(2);
    display->print("Sending");
    display->setCursor(20,31);
    display->print("new card");
    display->setCursor(33,48);
    display->print("to DB");
    display->display();

    delay(250);
    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);
    delay(250);
    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);
    delay(250);
    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);
    delay(250);
    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);
    delay(250);
    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);
    delay(250);
    digitalWrite(GREEN, HIGH);
    delay(250);
    digitalWrite(GREEN, LOW);

    state = ACTIVE;  
}

void oled::vInitCalibration()
{
    digitalWrite(GREEN, HIGH);
    digitalWrite(RED, LOW);
    display->clearDisplay();
    display->setTextSize(2);
    display->setCursor(15,16);
    display->setTextColor(WHITE);
    display->print("Starting");
    display->setCursor(50,32);
    display->print(".");
    display->display();
    delay(500);
    digitalWrite(GREEN, LOW);
    delay(500);
    digitalWrite(GREEN, HIGH);
    display->setCursor(60,32);
    display->print(".");
    display->display();
    delay(500);
    digitalWrite(GREEN, LOW);
    delay(500);
    digitalWrite(GREEN, HIGH);
    display->setCursor(70,32);
    display->print(".");
    display->display();
    delay(500);
    digitalWrite(GREEN, LOW);
    display->display();
}
void oled::vKnownWeight()
{

  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);

  display->clearDisplay();
  display->setTextSize(2);
  display->setCursor(30,0);
  display->setTextColor(WHITE);
  display->print("Weight");
  display->display();

  display->setTextSize(2);
  display->setCursor(30, 30);
  display->setTextColor(WHITE);
  display->print(knownWeight);
  display->setCursor(80, 30);
  display->print("gr");
  display->display();

  while (!selectWeight){
    if (calibrateWeight == true) {
      display->clearDisplay();
      display->setTextSize(2);
      display->setCursor(30,0);
      display->setTextColor(WHITE);
      display->print("Weight");
      display->display();

      display->setTextSize(2);
      display->setCursor(30, 30);
      display->setTextColor(WHITE);
      display->print(knownWeight);
      display->setCursor(80, 30);
      display->print("gr");
      display->display();
      vTaskDelay(1);

      calibrateWeight = false;
    } 
    vTaskDelay(50);
  }



  Serial.printf("[oled.cpp]: Select Weight: ");
  Serial.println(knownWeight);
  
}
void oled::vCalibrate()
{
  digitalWrite(RED, HIGH);
  vInitCalibration();
  digitalWrite(RED, LOW);

  state = SELECT_WEIGHT;

  vKnownWeight(); 

  vTaskDelay(3000);

  state = CELL_A; 
}

void oled::vCellCalibration(int cell_number)
{
    display->clearDisplay();

    String actualCell[5] = {"","CELL A","CELL B","CELL C","CELL D"};

    display->fillRoundRect(0,0,127,16,3,WHITE);
    display->setCursor(29,1);
    display->setTextSize(2);
    display->setTextColor(BLACK);
    display->print(actualCell[cell_number]);

    if (cell_number == 1) display->fillCircle(15,30,10,WHITE);
    else display->drawCircle(15,30,10,WHITE);
    if (cell_number == 2) display->fillCircle(40,30,10,WHITE);
    else display->drawCircle(40,30,10,WHITE);
    if (cell_number == 3) display->fillCircle(15,53,10,WHITE);
    else display->drawCircle(15,53,10,WHITE);
    if (cell_number == 4) display->fillCircle(40,53,10,WHITE);
    else display->drawCircle(40,53,10,WHITE);
    
    display->setCursor(56,20);
    display->setTextSize(1);
    display->setTextColor(WHITE);
    display->print ("Put a known");
    display->setCursor(56,32);
    display->print ("weigth and");
    display->setCursor(56,44);
    display->print ("press Select");
    display->setCursor(56,56);
    display->print ("button");

    display->display();
}

void oled::vCalibrated()
{
    display->clearDisplay();
    display->setCursor(3,31);
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->print("Calibrated");
    
    display->display();
}

void oled::vCheckOledState()
{
  switch (state){
  case ACTIVE:
    if (previousState != ACTIVE) {vDrawBalance(); previousState = ACTIVE;}
    if (actualice == A) { 
      display->fillRoundRect(20, 23, 40, 13, 1, WHITE);
      vDrawWeigth(int(1000*w1), 0, 0); actualice = X;
    }
    if (actualice == B) { 
      display->fillRoundRect(67, 23, 40, 13, 1, WHITE);
      vDrawWeigth(int(1000*w2), 1, 0); actualice = X; 
    }
    if (actualice == C) { 
      display->fillRoundRect(20, 43, 40, 13, 1, WHITE);
      vDrawWeigth(int(1000*w3), 0, 1); actualice = X; 
    }
    if (actualice == D) { 
      display->fillRoundRect(67, 43, 40, 13, 1, WHITE);
      vDrawWeigth(int(1000*w4), 1, 1); actualice = X; 
    }
    break;
  case CELL_A:
    digitalWrite(RED, HIGH);
    vCellCalibration(CELL_A);
    break;
  case CELL_B:
    vCellCalibration(CELL_B);
    break;
  case CELL_C:
    vCellCalibration(CELL_C);
    break;
  case CELL_D:
    vCellCalibration(CELL_D);
    break;
  case CAL:
    vCalibrate();
  break;
  case SEND_DATA:
    digitalWrite(GREEN, HIGH);
    vSendData();
    state = ACTIVE; previousState = SEND_DATA;
    digitalWrite(GREEN, LOW);
  break;
  case BALANCE:
    vTaskDelay(1);
    break;
  case SELECT_WEIGHT:
    vTaskDelay(1);
    break;
  default:
    break;
  }
}

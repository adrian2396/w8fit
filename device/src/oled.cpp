#include "oled.h"

extern float w1;
extern float w2;
extern float w3;
extern float w4;


oled::oled(Adafruit_SSD1306 *display_){
    display = display_;
}

oled::~oled(){
}


void oled::vOledBegin(){
    if(!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

void oled::vDrawBalance() {
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

void oled::vDrawWeigth(int weigth, int i, int j ){

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

void oled::vSelectCell(int w1, int w2, int w3, int w4,int cell_number){
  switch (cell_number){
  case CELL_A:
    vDrawWeigth(w1, 0, 0);
    break;
  case CELL_B:
    vDrawWeigth(w2, 0, 1);
    break;
  case CELL_C:
    vDrawWeigth(w3, 1, 0);
    break;
  case CELL_D:
    vDrawWeigth(w4, 1, 1);
    break;
  default:
    break;
  }

}

void oled::vSendData(){
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
}

void oled::vInitCalibration(){
    display->clearDisplay();
    display->setCursor(22,16);
    display->setTextColor(WHITE);
    display->print("Starting");
    display->setCursor(50,32);
    display->print(".");
    display->display();
    delay(1000);
    display->setCursor(60,32);
    display->print(".");
    display->display();
    delay(1000);
    display->setCursor(70,32);
    display->print(".");
    display->display();
    delay(1000);

    display->display();
}

void oled::vCellCalibration(int cell_number){
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

void oled::vCalibrated(){
    display->clearDisplay();
    display->setCursor(3,31);
    display->setTextSize(2);
    display->setTextColor(WHITE);
    display->print("Calibrated");
    
    display->display();
}

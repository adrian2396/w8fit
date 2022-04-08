#include "buttons.h"

int newCalibrate = false;
int newSelect = false;

extern int state;
extern int next;

extern int oledState;
extern int previousState;

extern int selectWeight;
extern float knownWeight;
extern int calibrateWeight;


buttons::buttons()
{
}

buttons::~buttons()
{
}

void buttons::vCheckSelect()
{
	if (digitalRead(SELECT) == LOW){
		newSelect = false;
	}
    if (digitalRead(SELECT) == HIGH) {
		if (!newSelect){
			Serial.println("[output.cpp]: BUTTON SELECT PRESSED ");
			newSelect = true;
			switch (state){
				case ACTIVE:
					Serial.println("[output.cpp]: send new card to database");
					state = SEND_DATA;
					previousState = SEND_DATA;
					break;
				case CELL_A:
					Serial.println("[output.cpp]: CELL A calibrated. Starting calibrate CELL B");
					next =  true;
					break;
				case CELL_B:
					Serial.println("[output.cpp]: CELL B calibrated. Starting calibrate CELL C");
					next =  true;
					break;
				case CELL_C:
					Serial.println("[output.cpp]: CELL C calibrated. Starting calibrate CELL D");
					next =  true;
					break;
				case CELL_D:
					Serial.println("[output.cpp]: CELL D calibrated. Preparing device to measure");
					next =  true;
					break;
				case CAL:
					vTaskDelay(1);
					break;
				case SEND_DATA:
					vTaskDelay(1);
					break;
				case BALANCE:
					vTaskDelay(1);
					break;
				case SELECT_WEIGHT:
					selectWeight = true;
					break;
				default:
					break;
			}
			vTaskDelay(100);	
		}
    }
}
void buttons::vCheckCalibrate()
{
	if (digitalRead(CALIBRATE) == LOW){
		newCalibrate = false;
	}
    if (digitalRead(CALIBRATE) == HIGH) {
        if (!newCalibrate){	
			Serial.println("[output.cpp]: BUTTON CALIBRATE PRESSED ");
			newCalibrate = true;

			switch (state){
				case ACTIVE:
					Serial.println("[output.cpp]: Starting calibration ");
					state = CAL;
					vTaskDelay(3000);
					state = SELECT_WEIGHT;
					break;
				case CELL_A:
					Serial.println("[output.cpp]: CELL A calibrated. Starting calibrate CELL B");
					next =  true;
					break;
				case CELL_B:
					Serial.println("[output.cpp]: CELL B calibrated. Starting calibrate CELL C");
					next =  true;
					break;
				case CELL_C:
					Serial.println("[output.cpp]: CELL C calibrated. Starting calibrate CELL D");
					next =  true;
					break;
				case CELL_D:
					Serial.println("[output.cpp]: CELL D calibrated. Preparing device to measure");
					next =  true;
					break;
				case CAL:
					vTaskDelay(1);
					break;
				case SEND_DATA:
					vTaskDelay(1);
					break;
				case BALANCE:
					vTaskDelay(1);
					break;
				case SELECT_WEIGHT:
					calibrateWeight = true;
					if (knownWeight <= 1) knownWeight = knownWeight + 0.05;
					else knownWeight = 0.05;
					break;
				default:
					vTaskDelay(1);
					break;
			}
			vTaskDelay(100);
		}
    }
}

void buttons::vActualizeOutputs()
{
    vCheckSelect();
    vCheckCalibrate();
}



void buttons::vPrintState()
{
	switch (state)
	{
	case ACTIVE:
		Serial.println("[buttons.cpp]: active");
		break;
	case CELL_A: 
		Serial.println("[buttons.cpp]: calibrating cell A");
		break;
	case CELL_B:
		Serial.println("[buttons.cpp]: calibrating cell B");
		break;
	case CELL_C:
		Serial.println("[buttons.cpp]: calibrating cell C");
		break;
	case CELL_D:
		Serial.println("[buttons.cpp]: calibrating cell D");
		break;
	case CAL:
		Serial.println("[buttons.cpp]: calibrated");
		break;
	case SEND_DATA:
		Serial.println("[buttons.cpp]: sending data");
		break;
	default:
		break;
	}
}


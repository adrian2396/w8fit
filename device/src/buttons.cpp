#include "buttons.h"

int newCalibrate = false;
int newSelect = false;

extern int state;
extern int next;

buttons::buttons(){
}

buttons::~buttons(){
}

void buttons::vCheckStateSelect(){
	Serial.println("[output.cpp]: ENNNNTRAAAA ");
	digitalWrite(GREEN, HIGH);
	digitalWrite(RED, HIGH);

	switch (state){
		case ACTIVE:
			Serial.println("[output.cpp]: send new card to database");
			break;
		case CELL_A:
			Serial.println("[output.cpp]: Starting calibrate CELL A");
			next =  true;
			break;
		case CELL_B:
			Serial.println("[output.cpp]: CELL A calibrated. Starting calibrate CELL B");
			next =  true;
			break;
		case CELL_C:
			Serial.println("[output.cpp]: CELL B calibrated. Starting calibrate CELL C");
			next =  true;
			break;
		case CELL_D:
			Serial.println("[output.cpp]: CELL C calibrated. Starting calibrate CELL D");
			next =  true;
			break;
		default:
			break;
	}

	digitalWrite(GREEN, LOW);
	digitalWrite(RED, LOW);
}
void buttons::vCheckStateCalibrate(){
	Serial.println("[output.cpp]: ENTRAAA 2 ");
	digitalWrite(GREEN, HIGH);
	digitalWrite(RED, HIGH);

	switch (state){
		case ACTIVE:
			Serial.println("[output.cpp]: Starting calibration ");
			state = CELL_A;
			break;
		default:
			vTaskDelay(1);
			break;
	}

	digitalWrite(GREEN, LOW);
	digitalWrite(RED, LOW);
}


void buttons::vCheckSelect(){
	if (digitalRead(SELECT) == LOW){
		newSelect = false;
	}
    if (digitalRead(SELECT) == HIGH) {
		if (!newSelect){
			Serial.println("[output.cpp]: BUTTON SELECT PRESSED ");
			newSelect = true;
			
			digitalWrite(GREEN, HIGH);
			digitalWrite(RED, HIGH);

			switch (state){
				case ACTIVE:
					Serial.println("[output.cpp]: send new card to database");
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
				default:
					break;
			}
			vTaskDelay(1000);

			digitalWrite(GREEN, LOW);
			digitalWrite(RED, LOW);			
		}
    }
}
void buttons::vCheckCalibrate(){
	if (digitalRead(CALIBRATE) == LOW){
		newCalibrate = false;
	}
    if (digitalRead(CALIBRATE) == HIGH) {
        if (!newCalibrate){	
			Serial.println("[output.cpp]: BUTTON CALIBRATE PRESSED ");
			newCalibrate = true;
			
			digitalWrite(GREEN, HIGH);
			digitalWrite(RED, HIGH);

			switch (state){
				case ACTIVE:
					Serial.println("[output.cpp]: Starting calibration ");
					state = CELL_A;
					break;
				default:
					vTaskDelay(1);
					break;
			}

			vTaskDelay(1000);

			digitalWrite(GREEN, LOW);
			digitalWrite(RED, LOW);

			
		}
    }
}

void buttons::vActualizeOutputs(){
    vCheckSelect();
    vCheckCalibrate();
}


/**
 *
 * HX711 library for Arduino
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#include <Arduino.h>
#include "HX711.h"

extern int state;
extern int next;

extern float w1;
extern float w2;
extern float w3;
extern float w4;



// TEENSYDUINO has a port of Dean Camera's ATOMIC_BLOCK macros for AVR to ARM Cortex M3.
#define HAS_ATOMIC_BLOCK (defined(ARDUINO_ARCH_AVR) || defined(TEENSYDUINO))

// Whether we are running on either the ESP8266 or the ESP32.
#define ARCH_ESPRESSIF (defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32))

// Whether we are actually running on FreeRTOS.
#define IS_FREE_RTOS defined(ARDUINO_ARCH_ESP32)

// Define macro designating whether we're running on a reasonable
// fast CPU and so should slow down sampling from GPIO.
#define FAST_CPU \
    ( \
    ARCH_ESPRESSIF || \
    defined(ARDUINO_ARCH_SAM)     || defined(ARDUINO_ARCH_SAMD) || \
    defined(ARDUINO_ARCH_STM32)   || defined(TEENSYDUINO) \
    )

#if HAS_ATOMIC_BLOCK
// Acquire AVR-specific ATOMIC_BLOCK(ATOMIC_RESTORESTATE) macro.
#include <util/atomic.h>
#endif

#if FAST_CPU

uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for(i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
    }
    return value;
}
#define SHIFTIN_WITH_SPEED_SUPPORT(data,clock,order) shiftInSlow(data,clock,order)
#else
#define SHIFTIN_WITH_SPEED_SUPPORT(data,clock,order) shiftIn(data,clock,order)
#endif

#ifdef ARCH_ESPRESSIF

#define DOUT_MODE INPUT
#else
#define DOUT_MODE INPUT_PULLUP
#endif


HX711::HX711() {
}

HX711::~HX711() {
}

void HX711::vBegin(byte dout, byte pd_sck, byte gain) {
	PD_SCK = pd_sck;
	DOUT = dout;

	pinMode(PD_SCK, OUTPUT);
	pinMode(DOUT, DOUT_MODE);

	vSetGain(gain);
}

bool HX711::vIsReady() {
	return digitalRead(DOUT) == LOW;
}

void HX711::vSetGain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}
}

long HX711::vRead() {

	// Wait for the chip to become ready.
	vWaitReady();

	// Define structures for reading data into.
	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	// Protect the read sequence from system interrupts.
	#if HAS_ATOMIC_BLOCK
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

	#elif IS_FREE_RTOS
	// Begin of critical section.
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	portENTER_CRITICAL(&mux);

	#else
	// Disable interrupts.
	noInterrupts();
	#endif

	// Pulse the clock pin 24 times to read the data.
	data[2] = SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
	data[1] = SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
	data[0] = SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);

	// Set the channel and the gain factor for the next reading using the clock pin.
	for (unsigned int i = 0; i < GAIN; i++) {
		digitalWrite(PD_SCK, HIGH);
		#if ARCH_ESPRESSIF
		delayMicroseconds(1);
		#endif
		digitalWrite(PD_SCK, LOW);
		#if ARCH_ESPRESSIF
		delayMicroseconds(1);
		#endif
	}

	#if IS_FREE_RTOS
	// End of critical section.
	portEXIT_CRITICAL(&mux);

	#elif HAS_ATOMIC_BLOCK
	}

	#else
	// Enable interrupts again.
	interrupts();
	#endif

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if (data[2] & 0x80) {
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = ( static_cast<unsigned long>(filler) << 24
			| static_cast<unsigned long>(data[2]) << 16
			| static_cast<unsigned long>(data[1]) << 8
			| static_cast<unsigned long>(data[0]) );

	return static_cast<long>(value);
}

void HX711::vWaitReady(unsigned long delay_ms) {
	// Wait for the chip to become ready.
	while (!vIsReady()) {
		//Serial.println("no ready");
		delay(delay_ms);
	}
}

bool HX711::vWaitReadyRetry(int retries, unsigned long delay_ms) {
	// Wait for the chip to become ready by
	// retrying for a specified amount of attempts.
	int count = 0;
	while (count < retries) {
		if (vIsReady()) {
			return true;
		}
		delay(delay_ms);
		count++;
	}
	return false;
}

bool HX711::vWaitReadyTimeout(unsigned long timeout, unsigned long delay_ms) {
	// Wait for the chip to become ready until timeout.
	unsigned long millisStarted = millis();
	while (millis() - millisStarted < timeout) {
		if (vIsReady()) {
			return true;
		}
		delay(delay_ms);
	}
	return false;
}

long HX711::vReadAverage(byte times) {
	long sum = 0;
	for (byte i = 0; i < times; i++) {
		sum += vRead();
		delay(0);
	}
	return sum / times;
}

double HX711::vGetValue(byte times) {
	return vReadAverage(times) - OFFSET;
}

float HX711::vGetUnits(byte times) {
	return vGetValue(times) / SCALE;
}

void HX711::vTare(byte times) {
	double sum = vReadAverage(times);
	vSetOffset(sum);
}

void HX711::vSetScale(float scale) {
	SCALE = scale;
}

float HX711::vGetScale() {
	return SCALE;
}

void HX711::vSetOffset(long offset) {
	OFFSET = offset;
}

long HX711::vGetOffset() {
	return OFFSET;
}

void HX711::vPowerDown() {
	digitalWrite(PD_SCK, LOW);
	digitalWrite(PD_SCK, HIGH);
}

void HX711::vPowerUp() {
	digitalWrite(PD_SCK, LOW);
}

void HX711::vCalibration(byte dout, byte sck, int hx711_n){

	vBegin(dout, sck);
	Serial.print("[hx711]: Calibrating DEVICE Nº ");
	Serial.println(hx711_n - 1);
	Serial.print("[hx711]: AD read:  ");
	Serial.println(vRead());
	vSetScale(); //La escala por defecto es 1
	vTare(20);  //El peso actual es considerado Tara.
	Serial.println("[hx711]: Put a known weight");

	for (int i = 0; i < 3; i++) {
		calibration = calibration +  vGetUnits(20);
	}

	calibration = calibration/10;

	while (!next){
		Serial.println("[hx711.cpp]: waiting for press select button");
		vTaskDelay(1000);
	}

	vBegin(dout, sck);
	Serial.print("[hx711]: AD2 read:   ");
	Serial.println(calibration = vRead());
	Serial.println("[hx711]: Put no weight on the scalea");
	Serial.println("[hx711]: Destarando...");
	vSetScale(SCALE = calibration/knownWeight); //La escala por defecto es 1
	vTare(20);  //El peso actual es considerado Tara.
	Serial.println("[hx711]: Calibrated Cell");

	next = false;
	
}
void HX711::vSetUp(byte dout, byte sck){
	vBegin(dout, sck);
	Serial.print("[hx711]: Lectura del valor del ADC:");
	Serial.println(vRead());
	vSetScale(924175); //La escala por defecto es 1
	vTare(20);  //El peso actual es considerado Tara.
	Serial.println("[hx711]: Coloque un peso conocido:");
}

void HX711::vCheckStateA(byte dout, byte clk){
	switch (state){
	case ACTIVE:
		w1 = 0;
		w1 = vGetUnits(20);
		Serial.print("[hx711]: Peso A: ");
		Serial.print(vGetUnits(20),1);
		Serial.println(" g");
		break;
	case CELL_A:
		/* Calibrate A */
		Serial.println("[hx711]: Calibrate A");
		vCalibration(dout, clk, CELL_A + 1);
		state = CELL_B;
		break;
	case CELL_B:
		vTaskDelay(1);
		break;
	case CELL_C:
		vTaskDelay(1);
		break;
	case CELL_D:
		vTaskDelay(1);
		break;
	default:
		break;
	}
	vTaskDelay(1000);
}
void HX711::vCheckStateB(byte dout, byte clk){
	switch (state){
	case ACTIVE:
		w2 = 0;
		w2 = vGetUnits(20);
		Serial.print("[hx711]: Peso B: ");
		Serial.print(vGetUnits(20),1);
		Serial.println(" g");
		break;
	case CELL_A:
		vTaskDelay(1);
		break;
	case CELL_B:
		/* Calibrate B */
		Serial.println("[hx711]: Calibrate B");
		vCalibration(dout, clk, CELL_B + 1);
		state = CELL_C;
		break;
	case CELL_C:
		vTaskDelay(1);
		break;
	case CELL_D:
		vTaskDelay(1);
		break;
	default:
		break;
	}
	vTaskDelay(1000);
}
void HX711::vCheckStateC(byte dout, byte clk){
	switch (state){
	case ACTIVE:
		w3 = 0;
		//w3 = vGetUnits(20);
		Serial.print("[hx711]: Peso C: ");
		//Serial.print(vGetUnits(20),1);
		Serial.println(" g");
		break;
	case CELL_A:
		vTaskDelay(1);
		break;
	case CELL_B:
		vTaskDelay(1);
		break;
	case CELL_C:
		/* Calibrate C */
		Serial.println("[hx711]: Calibrate C");
		vCalibration(dout, clk, CELL_C + 1);
		state = CELL_D;
		break;
	case CELL_D:
		vTaskDelay(1);
		break;
	default:
		break;
	}
	vTaskDelay(1000);
}
void HX711::vCheckStateD(byte dout, byte clk){
		switch (state){
		case ACTIVE:
			w4 = 0;
			w4 = vGetUnits(20);
			Serial.print("[hx711]: Peso D: ");
			Serial.print(vGetUnits(20),1);
			Serial.println(" g");
			break;
		case CELL_A:
			vTaskDelay(1);
			break;
		case CELL_B:
			vTaskDelay(1);
			break;
		case CELL_C:
			vTaskDelay(1);
			break;
		case CELL_D:
			/* Calibrate D */
			Serial.println("[hx711]: Calibrate D");
			vCalibration(dout, clk, CELL_D + 1);
			state = ACTIVE;
			break;
		default:
			break;
		}
		vTaskDelay(1000);
}



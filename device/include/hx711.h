/**
 *
 * HX711 library for Arduino
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#ifndef HX711_h
#define HX711_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "parameters.h"

#define CELL_NUMBER	4
#define SELECT     14 
#define CALIBRATE   27
#define GREEN   19
#define RED 18

class HX711
{
	private:
		byte PD_SCK;	// Power Down and Serial Clock Input Pin
		byte DOUT;		// Serial Data Output Pin
		byte GAIN;		// amplification factor
		long OFFSET = 0;
		/* calibration */
		float SCALE = 1;
		int calibration;
		float knownWeight = 0.2;

	public:

		HX711();

		virtual ~HX711();

		void vBegin(byte dout, byte pd_sck, byte gain = 128);
		bool vIsReady();
		void vWaitReady(unsigned long delay_ms = 0);
		bool vWaitReadyRetry(int retries = 3, unsigned long delay_ms = 0);
		bool vWaitReadyTimeout(unsigned long timeout = 1000, unsigned long delay_ms = 0);
		void vSetGain(byte gain = 128);
		long vRead();
		long vReadAverage(byte times = 10);
		double vGetValue(byte times = 1);
		float vGetUnits(byte times = 1);
		void vTare(byte times = 10);
		void vSetScale(float scale = 1.f);
		float vGetScale();
		void vSetOffset(long offset = 0);
		long vGetOffset();
		void vPowerDown();
		void vPowerUp();

		void vSetUp(byte dout, byte sck);
		void vCalibration(byte dout, byte sck, int hx711_n);

		void vCheckStateA(byte dout, byte clk);
		void vCheckStateB(byte dout, byte clk);
		void vCheckStateC(byte dout, byte clk);
		void vCheckStateD(byte dout, byte clk);
};

#endif /* HX711_h */
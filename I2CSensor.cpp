// Example program connecting to the TCS34725 Color Sensor to the K64F using I2C

#include "mbed.h"
#include "TCS34725.h"
#include "I2CSensor.h"



int tcsAddr = 41 << 1;

ColorData colorData;
TCS34725 *tcs=0;


Thread threadI2C(osPriorityNormal, 1024); // 1K stack size

void I2C_thread();

void I2C_thread() {
		tcs = new TCS34725(PB_9,PB_8,tcsAddr, PB_14);
		
	
		if(tcs->checkId()){
			tcs->initColorSensor();
		}
		while (true) {
			colorData = tcs->readRegisters();
			wait(0.25);
    }
}


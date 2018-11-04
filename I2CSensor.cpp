// Example program connecting to the TCS34725 Color Sensor to the K64F using I2C

#include "mbed.h"
#include "TCS34725.h"
#include "MMA8451Q.h"
#include "Si7021.h"



int tcsAddr = 41 << 1;
int mmaAddr = 0x1D;
int si7Addr = 0x40 << 1;

ColorData colorData;
AccelerometerData accData;
AmbientData ambData;
TCS34725 *tcs=0;
MMA8451Q *acc=0;
Si7021 *amb=0;



Thread threadI2C(osPriorityNormal, 1024); // 1K stack size

void I2C_thread();

void I2C_thread() {
		tcs = new TCS34725(PB_9,PB_8,tcsAddr, PB_14);
		acc = new MMA8451Q(PB_9,PB_8,mmaAddr);
		amb = new Si7021(PB_9,PB_8,si7Addr);
	
		if(tcs->checkId()){
			tcs->initColorSensor();
		}
		while (true) {
			colorData = tcs->readRegisters();
			accData = acc->getAccAllAxis();
			ambData = amb->measure();
			wait(0.5);
    }
}

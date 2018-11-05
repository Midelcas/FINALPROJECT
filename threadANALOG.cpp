#include "mbed.h"
#include "rtos.h"
#include "HW5P-1.h"


//AnalogIn soilmois(PA_0); 

//float valueSM=0.0;

LightData lightData;
Thread threadANALOG(osPriorityNormal, 512); // 1K stack size

void ANALOG_thread(); 
HW5P_1 *lightSensor=0;

void ANALOG_thread() {
	lightSensor=new HW5P_1(PA_4);
	while (true) {
				lightData=lightSensor->measure();
				threadANALOG.signal_wait(0x1);
				//valueSM=soilmois*100;
				  
    }
}

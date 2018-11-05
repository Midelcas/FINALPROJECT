#include "mbed.h"
#include <string>
#include "rtos.h"
#include "TCS34725.h"
#include "MMA8451Q.h"
#include "Si7021.h"
#include "HW5P-1.h"
#include "SOIL.h"

#define OFF 0
#define ON 1
#define TIMEOUT_TEST_MODE    2
#define TIMEOUT_NORMAL_MODE 30

void setLed();	

Serial pc(USBTX,USBRX,9600);

BusOut leds(PH_0, PH_1, PB_13);
InterruptIn sw1(PB_2);
DigitalOut testMode(LED1);
DigitalOut normalMode(LED2);

extern Thread threadANALOG;
extern void ANALOG_thread();

extern Thread threadI2C;
extern void I2C_thread();

extern float valueSM;

extern ColorData colorData;
extern AccelerometerData accData;
extern AmbientData ambData;
extern LightData lightData;
extern SoilData soilData;
Timeout to;
bool writeTime;
int count;

void timeToWrite(void){
	writeTime=1;
	count++;
	threadI2C.signal_set(0x1);
	threadANALOG.signal_set(0x1);
	if(testMode==ON){
		to.attach_us(timeToWrite,TIMEOUT_TEST_MODE*1000000);
	}else if(normalMode==ON){
		to.attach_us(timeToWrite,TIMEOUT_NORMAL_MODE*1000000);
	}
}

void switch_handler(void){
	testMode=!testMode;
	normalMode=!normalMode;
	if(testMode==ON){
		to.attach_us(timeToWrite,TIMEOUT_TEST_MODE*1000000);
	}else if(normalMode==ON){
		count=0;
		to.attach_us(timeToWrite,TIMEOUT_NORMAL_MODE*1000000);
	}
}

// main() runs in its own thread in the OS
int main() {
		count=0;
		writeTime=0;
		testMode=ON;
		normalMode=OFF;
		to.attach_us(timeToWrite,TIMEOUT_TEST_MODE*1000000); 

    threadANALOG.start(ANALOG_thread);
		threadI2C.start(I2C_thread);
		sw1.mode(PullUp);
		sw1.fall(switch_handler);

	
    pc.printf("mbed-os-rev: %d.%d.%d\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);	
	
    while (true) {

			if(writeTime){
				pc.printf("\n\r");
				if(testMode==ON){
					pc.printf("\nTemp: (%.2fºC),Hum: (%.2f%%)\n",ambData.temperature, ambData.humidity);
					pc.printf("\nX (%f),Y (%f),Z (%f)\n",accData.x, accData.y, accData.z);
					pc.printf("\nClear (%d)Red (%d), Green (%d), Blue (%d) \n",colorData.clear_value, colorData.red_value, 
								colorData.green_value, colorData.blue_value);
					leds = colorData.dominant;
					pc.printf("\nLight (%f%%)\n",lightData.light);
					pc.printf("\nSoil Moisture (%f%%)\n",soilData.soil);
				}else if(normalMode==ON){
					pc.printf("\nTemp: (%.2fºC),Hum: (%.2f%%)\n",ambData.temperature, ambData.humidity);
					pc.printf("\nX (%f),Y (%f),Z (%f)\n",accData.x, accData.y, accData.z);
					pc.printf("\nClear (%d)Red (%d), Green (%d), Blue (%d) \n",colorData.clear_value, colorData.red_value, 
								colorData.green_value, colorData.blue_value);
					pc.printf("\nLight (%f%%)\n",lightData.light);
					pc.printf("\nSoil Moisture (%f%%)\n",soilData.soil);
					if(count==120){
						pc.printf("\nMaxH: (%.2f%%),MinH: (%.2f%%)\n",ambData.maxHumidity, ambData.minHumidity);
						pc.printf("\nMaxT: (%.2fºC),MinT: (%.2fºC)\n",ambData.maxTemperature, ambData.minTemperature);
						pc.printf("\nMeanH: (%.2f%%),MeanT: (%.2fºC)\n",ambData.meanHumidity, ambData.meanTemperature);
						
						pc.printf("\nMaxX (%f),MaxY (%f),MaxZ (%f)\n",accData.x_Max, accData.y_Max, accData.z_Max);
						pc.printf("\nMinX (%f),MinY (%f),MinZ (%f)\n",accData.x_Min, accData.y_Min, accData.z_Min);
						pc.printf("\nDominant Color:%s\n",colorData.hourDominant.c_str());
						
						pc.printf("\nMax Light (%f%%), Min Light (%f%%), Mean Light (%f%%\n",lightData.maxLight, lightData.minLight, lightData.meanLight);
						pc.printf("\nMax Soil Moisture(%f%%), Min Soil Moisture(%f%%), Mean Soil Moisture(%f%%\n",soilData.maxSoil, soilData.minSoil, soilData.meanSoil);
						//pc.printf("\nRedTimes (%d),GreenTimes (%d),BlueTimes (%d)\n",colorData.acc_red, colorData.acc_green, colorData.acc_blue);
					}
					
				}
				writeTime=0;
			}		
			wait(.1);
			
    }
}

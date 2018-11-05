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
#define TEMP_MAX 	24
#define TEMP_MIN	18
#define HUM_MAX		50
#define HUM_MIN		40
#define LIGHT_MAX	70
#define LIGHT_MIN	40
#define SOIL_MAX	60
#define SOIL_MIN	40
#define RED_MAX		3000
#define RED_MIN		30
#define GREEN_MAX	3000
#define GREEN_MIN	30
#define BLUE_MAX	3000
#define BLUE_MIN	30
#define ACC_MAX_X	1.5
#define ACC_MIN_X	-0.5
#define ACC_MAX_Y	1.5
#define ACC_MIN_Y	-0.5
#define ACC_MAX_Z	2
#define ACC_MIN_Z	0.5

Serial pc(USBTX,USBRX,9600);
BusOut leds(PH_0, PH_1, PB_13);
InterruptIn sw1(PB_2);
DigitalOut testMode(LED1);
DigitalOut normalMode(LED2);
Ticker tick;

extern Thread threadANALOG;
extern void ANALOG_thread();

extern Thread threadI2C;
extern void I2C_thread();


extern ColorData colorData;
extern AccelerometerData accData;
extern AmbientData ambData;
extern LightData lightData;
extern SoilData soilData;


bool writeTime;
int count;

void timeToWrite(void){
	writeTime=1;
	count++;
	threadI2C.signal_set(0x1);
	threadANALOG.signal_set(0x1);
}

void switch_handler(void){
	testMode=!testMode;
	normalMode=!normalMode;
	tick.detach();
	if(testMode==ON){
		tick.attach_us(timeToWrite,TIMEOUT_TEST_MODE*1000000);
	}else if(normalMode==ON){
		count=0;
		tick.attach_us(timeToWrite,TIMEOUT_NORMAL_MODE*1000000);
	}
}
//0 WHITE
//1 YELLOW
//2 PURPLE
//3 RED
//4 LIGHT BLUE
//5 GREEN
//6 BLUE
//7 OFF
void checkLimits(){
	if((ambData.temperature>TEMP_MAX)||(ambData.temperature<TEMP_MIN)){
		leds=3;//RED
	}else if((ambData.humidity>HUM_MAX)||(ambData.humidity<HUM_MIN)){
		leds=6;//BLUE
	} else if((soilData.soil>SOIL_MAX)||(soilData.soil<SOIL_MIN)){
		leds=4;//LIGHT BLUE
	} else if((lightData.light>LIGHT_MAX)||(lightData.light<LIGHT_MIN)){
		leds=1;//YELLOW
	} else if((colorData.red_value>RED_MAX)||(colorData.red_value<RED_MIN)
		||(colorData.green_value>GREEN_MAX)||(colorData.green_value<GREEN_MIN)
		||(colorData.blue_value>BLUE_MAX)||(colorData.blue_value<BLUE_MIN)){
		leds=5;//GREEN
	} else if((accData.x>ACC_MAX_X)||(accData.x<ACC_MIN_X)
		||(accData.y>ACC_MAX_Y)||(accData.y<ACC_MIN_Y)
		||(accData.z>ACC_MAX_Z)||(accData.z<ACC_MIN_Z)){
		leds=2;//PURPLE
	}
}

void printMeasures(){
	pc.printf("\nTemp: (%.2fºC),Hum: (%.2f%%)\n",ambData.temperature, ambData.humidity);
	pc.printf("\nX (%f),Y (%f),Z (%f)\n",accData.x, accData.y, accData.z);
	pc.printf("\nClear (%d)Red (%d), Green (%d), Blue (%d) \n",colorData.clear_value, colorData.red_value, 
				colorData.green_value, colorData.blue_value);
	leds = colorData.dominant;
	pc.printf("\nLight (%.2f%%)\n",lightData.light);
	pc.printf("\nSoil Moisture (%.2f%%)\n",soilData.soil);
}

void printHour(){
	checkLimits();
	pc.printf("\nMaxH: (%.2f%%),MinH: (%.2f%%)\n",ambData.maxHumidity, ambData.minHumidity);
	pc.printf("\nMaxT: (%.2fºC),MinT: (%.2fºC)\n",ambData.maxTemperature, ambData.minTemperature);
	pc.printf("\nMeanH: (%.2f%%),MeanT: (%.2fºC)\n",ambData.meanHumidity, ambData.meanTemperature);
	
	pc.printf("\nMaxX (%f),MaxY (%f),MaxZ (%f)\n",accData.x_Max, accData.y_Max, accData.z_Max);
	pc.printf("\nMinX (%f),MinY (%f),MinZ (%f)\n",accData.x_Min, accData.y_Min, accData.z_Min);
	pc.printf("\nDominant Color:%s\n",colorData.hourDominant.c_str());
	
	pc.printf("\nMax Light (%.2f%%), Min Light (%.2f%%), Mean Light (%.2f%%\n",lightData.maxLight, lightData.minLight, lightData.meanLight);
	pc.printf("\nMax Soil Moisture(%.2f%%), Min Soil Moisture(%.2f%%), Mean Soil Moisture(%.2f%%\n",soilData.maxSoil, soilData.minSoil, soilData.meanSoil);
}

// main() runs in its own thread in the OS
int main() {
		count=0;
		writeTime=0;
		testMode=ON;
		normalMode=OFF;
	
		tick.attach_us(timeToWrite,TIMEOUT_TEST_MODE*1000000);
    threadANALOG.start(ANALOG_thread);
		threadI2C.start(I2C_thread);
		sw1.mode(PullUp);
		sw1.fall(switch_handler);

	
    pc.printf("mbed-os-rev: %d.%d.%d\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);	
	
    while (true) {

			if(writeTime){
				pc.printf("\n\r");
				if(testMode==ON){
					printMeasures();
				}else if(normalMode==ON){
					printMeasures();
					if(count==120){
						printHour();
						count =0;
					}
					
				}
				writeTime=0;
			}		
			wait(.1);
			
    }
}

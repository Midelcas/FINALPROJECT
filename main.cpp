#include "mbed.h"
#include "rtos.h"
#include "TCS34725.h"
#include "MMA8451Q.h"
#include "Si7021.h"


void setLed();	

Serial pc(USBTX,USBRX,9600);

BusOut leds(PH_0, PH_1, PB_13);

//extern Thread threadANALOG;

//extern void ANALOG_thread();

extern Thread threadI2C;

extern void I2C_thread();

extern float valueSM;

extern ColorData colorData;
extern AccelerometerData accData;
extern AmbientData ambData;



// main() runs in its own thread in the OS
int main() {


    //threadANALOG.start(ANALOG_thread);
		threadI2C.start(I2C_thread);

	
    pc.printf("mbed-os-rev: %d.%d.%d\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);	
	
    while (true) {

			pc.printf("\n\r");
		
			// SOIL MOISTURE
			//pc.printf("\n\rSOIL MOISTURE: %.1f%%",valueSM);
			pc.printf("\nMaxH: (%.2f%%),MinH: (%.2f%%)\n",ambData.maxHumidity, ambData.minHumidity);
			pc.printf("\nMaxT: (%.2fºC),MinT: (%.2fºC)\n",ambData.maxTemperature, ambData.minTemperature);
			pc.printf("\nMeanH: (%.2f%%),MeanT: (%.2fºC)\n",ambData.meanHumidity, ambData.meanTemperature);
			pc.printf("\nTemp: (%.2fºC),Hum: (%.2f%%)\n",ambData.temperature, ambData.humidity);
			pc.printf("\nMaxX (%f),MaxY (%f),MaxZ (%f)\n",accData.x_Max, accData.y_Max, accData.z_Max);
			pc.printf("\nMinX (%f),MinY (%f),MinZ (%f)\n",accData.x_Min, accData.y_Min, accData.z_Min);
			pc.printf("\nX (%f),Y (%f),Z (%f)\n",accData.x, accData.y, accData.z);
			pc.printf("\nRedTimes (%d),GreenTimes (%d),BlueTimes (%d)\n",colorData.acc_red, colorData.acc_green, colorData.acc_blue);
			pc.printf("\nClear (%d)Red (%d), Green (%d), Blue (%d) \n",colorData.clear_value, colorData.red_value, 
								colorData.green_value, colorData.blue_value);
			leds = colorData.dominant;
			wait(1);
			
    }
}

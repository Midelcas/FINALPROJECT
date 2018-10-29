#include "mbed.h"
#include "rtos.h"
#include "ColorSensor.h"

	

Serial pc(USBTX,USBRX,9600);


//extern Thread threadANALOG;

//extern void ANALOG_thread();

extern Thread threadI2C;

extern void I2C_thread();

extern float valueSM;

extern ColorData color;



// main() runs in its own thread in the OS
int main() {


    //threadANALOG.start(ANALOG_thread);
		threadI2C.start(I2C_thread);

	
    pc.printf("mbed-os-rev: %d.%d.%d\r\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);	
	
    while (true) {

			  pc.printf("\n\r");
			
				// SOIL MOISTURE
			  //pc.printf("\n\rSOIL MOISTURE: %.1f%%",valueSM);
				pc.printf("\nClear (%d)Red (%d), Green (%d), Blue (%d) \n",color.clear_value, color.red_value, 
									color.green_value, color.blue_value);

        wait(2);
			
    }
}


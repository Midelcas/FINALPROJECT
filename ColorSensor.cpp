// Example program connecting to the TCS34725 Color Sensor to the K64F using I2C

#include "mbed.h"
#include "ColorSensor.h"

int sensor_addr = 41 << 1;

DigitalOut Led(PB_14);
I2C i2c(PB_9, PB_8); //pins for I2C communication (SDA, SCL)
extern Serial pc;


ColorData color;

Thread threadI2C(osPriorityNormal, 1024); // 1K stack size

void I2C_thread(); 
bool checkId();
void initColorSensor();
void readRegisters();

void I2C_thread() {
		Led = 0;
		i2c.frequency(100000);
	
		if(checkId()){
			initColorSensor();
		}
		while (true) {
			readRegisters();
			wait(0.25);
    }
}


bool checkId(){
	char id_regval[1] = {COMMAND|ID};//0x12 device ID
	char data[1] = {0};//response buffer
	i2c.write(sensor_addr,id_regval,1, true);//sets the control to reg 0x12
	i2c.read(sensor_addr,data,1,false);//reads from 0x12
	
	return (data[0]==TCS34725);
}

void initColorSensor(){
	char timing_register[2] = {COMMAND|ATIME,0x00};
	i2c.write(sensor_addr,timing_register,2,false);
	
	char control_register[2] = {COMMAND|CONTROL,0x00};
	i2c.write(sensor_addr,control_register,2,false);
	
	char enable_register[2] = {COMMAND|ENABLE,3};
	i2c.write(sensor_addr,enable_register,2,false);
}

void readRegisters(){
		Led = 1;
		wait(0.1);
		char color_reg[1]={COMMAND|CDATAL};
		char color_data[8] = {0};
		i2c.write(sensor_addr, color_reg, 1, true);
		i2c.read(sensor_addr,color_data,8,false);
		Led= 0;
			
		color.clear_value = ((int)color_data[1]<<8) | color_data[0];
		color.red_value = ((int)color_data[3] << 8) | color_data[2];
		color.green_value = ((int)color_data[5] <<8)| color_data[4];
		color.blue_value = ((int)color_data[7] <<8) | color_data[6];
}

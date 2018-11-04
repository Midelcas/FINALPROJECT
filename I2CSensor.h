

typedef struct{
	int clear_value;
	int red_value;
	int green_value;
	int blue_value;
	int acc_red;
	int acc_green;
	int acc_blue;
}ColorData;

typedef struct{
	float x;
	float x_Max;
	float x_Min;
	float y;
	float y_Max;
	float y_Min;
	float z;
	float z_Max;
	float z_Min;
}AccelerometerData;

typedef struct{
	float humidity;
	float maxHumidity;
	float minHumidity;
	float meanHumidity;
	float temperature;
	float maxTemperature;
	float minTemperature;
	float meanTemperature;
}AmbientData;

#define COMMAND 	0x80
#define ENABLE 		0x00
#define ATIME 		0x01
#define WTIME 		0x03
#define AILTL 		0x04
#define AILTH 		0x05
#define AIHTL 		0x06
#define AIHTH 		0x07
#define PERS 			0x0C
#define CONFIG 		0x0D
#define CONTROL 	0x0F
#define ID 				0x12
#define STATUS 		0x13
#define CDATAL 		0x14
#define CDATAH 		0x15
#define RDATAL 		0X16
#define RDATAH 		0X17
#define GDATAL 		0X18
#define GDATAH 		0X19
#define BDATAL 		0X1A
#define BDATAH 		0X1B
#define TCS34725 	0x44

#define RED		3
#define BLUE	6
#define GREEN	5

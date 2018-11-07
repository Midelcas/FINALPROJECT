//gps.cpp
//for use with Adafruit Ultimate GPS
//Reads in and parses GPS data

#include "mbed.h"
#include "SerialGPS.h"
#include "rtos.h"

#define USART1_TX PA_9
#define USART1_RX PA_10
#define GPS_BAUD 	9600
#define GPS_ENABLE PB_13

/*
struct StGPS {
    float longitude;
    float latitude;
    float time;
    int sats;
    float hdop;
    float alt;
    float  geoid;
		int fix;
};
*/


//SerialGPS myGPS(PA_9,PA_10, 9600);
SerialGPS myGPS(USART1_TX,USART1_RX, GPS_BAUD);
StGPS 		B1gps;

//int     	gpsTimeout;
//Serial pc2(USBTX,USBRX,19200);

Thread threadGPS(osPriorityNormal, 1024); // 1K stack size

void GPS_thread();


void GPS_thread(){
	int result = 0;	

	//Solo pruebas, quitarlo cuando funcione bien.
	//wait(gpsTimeout);								
	//pc2.printf("gps thread \n");
	
	//DigitalOut gpsEnable(GPS_ENABLE);

   while (true){
		 result = myGPS.sample();
		 //pc2.printf("gps thread, new sample ........ \n");
		 //pc2.printf("Msg: %c \n", myGPS.msg);
		 
		 B1gps.longitude = myGPS.longitude;
		 B1gps.latitude = myGPS.latitude;
		 B1gps.time = myGPS.time;
		 B1gps.sats = myGPS.sats;
		 B1gps.hdop = myGPS.hdop;
		 B1gps.alt = myGPS.alt;
		 B1gps.geoid = myGPS.geoid;
		 B1gps.fix = result;
		 
		 //Thread::signal_set(0x1);			// Lanzar todos los threads al terminar la lectura serie
		 threadGPS.signal_wait(0x2);	
	 } 	
}








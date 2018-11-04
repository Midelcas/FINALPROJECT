/**
 * MMA8451Q 3-Axis, 14-bit/8-bit Digital Accelerometer
 */
 
#include "mbed.h"
#include "MMA8451Q.h"

#define REG_STATUS           0x00 // when F_MODE = 00
#define REG_FIFO_STATUS      0x00 // when F_MODE > 0
#define REG_XYZ_FIFO         0x01 // Root pointer to XYZ FIFO data
#define REG_OUT_X_MSB        0x01 // 8 MSBs of 14-bit sample
#define REG_OUT_X_LSB        0x02 // 6 LSBs of 14-bit sample
#define REG_OUT_Y_MSB        0x03 
#define REG_OUT_Y_LSB        0x04
#define REG_OUT_Z_MSB        0x05
#define REG_OUT_Z_LSB        0x06
#define REG_F_SETUP          0x09 // FIFO setup
#define REG_TRIG_CFG         0x0A // Map of FIFO daa capture events
#define REG_SYSMOD           0x0B // Current System Mode
#define REG_INT_SOURCE       0x0C // Interrupt status
#define REG_WHO_AM_I         0x0D // Device ID (0x1A)
#define REG_XYZ_DATA_CFG     0x0E // Dynamic Range Settings
#define REG_HP_FILTER_CUTOFF 0x0F // Cutoff freq is set to 16Hz@800Hz
#define REG_PL_STATUS        0x10 // Landscape/Portrait orientation status
#define REG_PL_CFG           0x11 // Landscape/Portrait configuration
#define REG_PL_COUNT         0x12 // Landscape/Portrait debounce counter 
#define REG_PL_BF_ZCOMP      0x13 // Back/Front, Z-Lock Trip threshold
#define REG_P_L_THS_REG      0x14 // Portrait to Landscape Trip Angle is 29 degree
#define REG_FF_MT_CFG        0x15 // Freefall/Motion function block configuration
#define REG_FF_MT_SRC        0x16 // Freefall/Motion event source register
#define REG_FF_MT_THS        0x17 // Freefall/Motion threshold register
#define REG_FF_MT_COUNT      0x18 // Freefall/Motion debounce counter
// TRANSIENT
#define REG_TRANSIENT_CFG    0x1D // Transient functional block configuration
#define REG_TRANSIENT_SRC    0x1E // Transient event status register
#define REG_TRANSIENT_THS    0x1F // Transient event threshold
#define REG_TRANSIENT_COUNT  0x20 // Transient debounce counter
// PULSE
#define REG_PULSE_CFG        0x21 // ELE, Double_XYZ or Single_XYZ
#define REG_PULSE_SRC        0x22 // EA, Double_XYZ or Single_XYZ
#define REG_PULSE_THSX       0x23 // X pulse threshold
#define REG_PULSE_THSY       0x24 // Y pulse threshold
#define REG_PULSE_THSZ       0x25 // Z pulse threshold
#define REG_PULSE_TMLT       0x26 // Time limit for pulse
#define REG_PULSE_LTCY       0x27 // Latency time for 2nd pulse
#define REG_PULSE_WIND       0x28 // Window time for 2nd pulse
#define REG_ASLP_COUNT       0x29 // Counter setting for Auto-SLEEP
// Control Registers
#define REG_CTRL_REG1        0x2A // ODR = 800Hz, STANDBY Mode
#define REG_CTRL_REG2        0x2B // Sleep Enable, OS Modes, RST, ST
#define REG_CTRL_REG3        0x2C // Wake from Sleep, IPOL, PP_OD
#define REG_CTRL_REG4        0x2D // Interrupt enable register
#define REG_CTRL_REG5        0x2E // Interrupt pin (INT1/INT2) map
// User Offset
#define REG_OFF_X            0x2F // X-axis offset adjust
#define REG_OFF_Y            0x30 // Y-axis offset adjust
#define REG_OFF_Z            0x31 // Z-axis offset adjust

// Value definitions
#define BIT_TRIG_TRANS       0x20  // Transient interrupt trigger bit
#define BIT_TRIG_LNDPRT      0x10  // Landscape/Portrati Orientation
#define BIT_TRIG_PULSE       0x08  // Pulse interrupt trigger bit
#define BIT_TRIG_FF_MT       0x04  // Freefall/Motion trigger bit

MMA8451Q::MMA8451Q(PinName sda, PinName scl, int addr) : m_i2c(sda, scl), m_addr(addr<<1) {
    // activate the peripheral
    uint8_t data[2] = {REG_CTRL_REG1, 0x01};
    writeRegs(data, 2);
		accData.x=0;
		accData.y=0;
		accData.z=0;
		accData.x_Max=0;
		accData.x_Min=0;
		accData.y_Max=0;
		accData.y_Min=0;
		accData.z_Max=0;
		accData.z_Min=0;
}

MMA8451Q::~MMA8451Q() { }

void MMA8451Q::readRegs(int addr, uint8_t * data, int len) 
{
    char t[1] = {addr};
    m_i2c.write(m_addr, t, 1, true);
    m_i2c.read(m_addr, (char *)data, len);
}

void MMA8451Q::writeRegs(uint8_t * data, int len) 
{
    m_i2c.write(m_addr, (char *)data, len);
}

int16_t MMA8451Q::getRawData(uint8_t addr)
{
    int16_t value ;
    uint8_t data[2] ;
    readRegs(addr, data, 2) ;
    value = ((int16_t)((data[0] << 8) | data[1])) >> 2 ;
    return( value ) ;
}

int16_t MMA8451Q::getRawX(void) 
{
    int16_t value ;
    value = getRawData(REG_OUT_X_MSB) ;
		//value  = (value <<8) + getRawData(REG_OUT_X_LSB);
    return( value ) ;
}

int16_t MMA8451Q::getRawY(void) 
{
    int16_t value ;
		value = getRawData(REG_OUT_Y_MSB) ;
		//value  = (value <<8) + getRawData(REG_OUT_Y_LSB);
    return( value ) ;
}

int16_t MMA8451Q::getRawZ(void) 
{
    int16_t value ;
    value = getRawData(REG_OUT_Z_MSB) ;
		//value  = (value <<8) + getRawData(REG_OUT_Z_LSB);
    return( value ) ;
}
  
float MMA8451Q::getAccX(void)
{
    return(((float)getRawX())/4096.0) ;
}

float MMA8451Q::getAccY(void) 
{
    return(((float)getRawY())/4096.0) ;
}

float MMA8451Q::getAccZ(void) 
{
    return(((float)getRawZ())/4096.0) ;
}

AccelerometerData MMA8451Q::getData(void){
	int16_t value ;
	uint8_t data[6] ;
	readRegs(REG_OUT_X_MSB, data, 6) ;
	accData.x = ((float)(((int16_t)((data[0] << 6) | data[1])) >> 2)/4096) ;
	accData.y = ((float)(((int16_t)((data[2] << 6) | data[3])) >> 2)/4096) ;
	accData.z = ((float)(((int16_t)((data[4] << 6) | data[5])) >> 2)/4096) ;
	
	if(accData.x>accData.x_Max)
		accData.x_Max=accData.x;
	if(accData.x<accData.x_Min)
		accData.x_Min=accData.x;
	if(accData.y>accData.y_Max)
		accData.y_Max=accData.y;
	if(accData.y<accData.y_Min)
		accData.y_Min=accData.y;
	if(accData.z>accData.z_Max)
		accData.z_Max=accData.z;
	if(accData.z<accData.z_Min)
		accData.z_Min=accData.z;
	
		return accData;
}

void MMA8451Q::setSingleTap(void){
		uint8_t data0[2] = {0x2A,0x08}; //400 Hz, Standby Mode. DR2=0 DR1=0 DR0=0 CTRL_REG1
		writeRegs(data0,2);
	
		uint8_t data1[2] = {0x21,0x15};//PULSE_CFG REGISTER 0x15 enables XSPEFE YSPEFE ZSPEFE (enables single pulse event for X, Y & Z)
		writeRegs(data1,2);
		
		uint8_t data2[2] = {0x23,0x10};//Threshold for X axis PULSE_THSX REGISTER
		writeRegs(data2,2);
		
		uint8_t data3[2] = {0x24,0x10};//Threshold for Y axis PULSE_THSY REGISTER
		writeRegs(data3,2);
		
		uint8_t data4[2] = {0x25,0x10};//Threshold for Z axis PULSE_THSZ REGISTER
		writeRegs(data4,2);
		
		uint8_t data5[2] = {0x26,0x50};//PULSE_TMLT (50ms)number of steps (time step depens on ODR frequency and power mode. FF_MT_COUNT Table 51.
		writeRegs(data5,2);
		
		uint8_t data6[2] = {0x27,0xF0};//PULSE_LTCY REGISTER 0xF0 VALUE FOR 300ms
		writeRegs(data6,2);
		
		uint8_t data7[2] = {0x2D,0x08};//CTRL_REG4 INT_EN_PULSE enabled to detect interrupts 
		writeRegs(data7,2);
		
		uint8_t data8[2] = {0x2E,0x08};//CTRL_REG5 INT_CFG_PULSE to 1 to route the interrupt to INT1 pint
		writeRegs(data8,2);
		
		uint8_t data9[2] ;
    readRegs(0x2A, data9, 2) ;
		data9[1]|=0x01;
		data9[0] = 0x2A;
		writeRegs(data9,2);
}

void MMA8451Q::setDobleTap(void){// 0x0F HP_FILTER_CUTOFF Pulse_LPF_EN default = 0; power mode
		uint8_t data0[2] = {0x2A,0x08};//400 Hz, Standby Mode. DR2=0 DR1=0 DR0=0 CTRL_REG1
		writeRegs(data0,2);
	
		uint8_t data1[2] = {0x21,0x2A};////PULSE_CFG REGISTER 0x2A enables XSPEFE YSPEFE ZSPEFE (enables doble pulse event for X, Y & Z)
		writeRegs(data1,2);
		
		uint8_t data2[2] = {0x23,0x10};//Threshold for X axis PULSE_THSX REGISTER
		writeRegs(data2,2);
		
		uint8_t data3[2] = {0x24,0x10};//Threshold for Y axis PULSE_THSY REGISTER
		writeRegs(data3,2);
		
		uint8_t data4[2] = {0x25,0x10};//Threshold for Z axis PULSE_THSZ REGISTER
		writeRegs(data4,2);
		
		uint8_t data5[2] = {0x26,0x30};//PULSE_TMLT number of steps (time step depens on ODR frequency and power mode. FF_MT_COUNT Table 51.
		writeRegs(data5,2);
		
		uint8_t data6[2] = {0x27,0x50};//PULSE_LTCY REGISTER 0xF0 VALUE FOR 300ms
		writeRegs(data6,2);
		
		uint8_t data7[2] = {0x28,0x78};//PULSE_WIND 
		writeRegs(data7,2);
		
		uint8_t data8[2] = {0x2D,0x08};//CTRL_REG4 INT_EN_PULSE enabled to detect interrupts 
		writeRegs(data8,2);
		
		uint8_t data9[2] = {0x2E,0x08};//CTRL_REG5 INT_CFG_PULSE to 1 to route the interrupt to INT1 pint
		writeRegs(data9,2);
		
		uint8_t data10[2] ;
    readRegs(0x2A, data10, 2) ;
		data10[1]|=0x01;
		data10[0] = 0x2A;
		writeRegs(data10,2);
}
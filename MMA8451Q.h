/**
 * MMA8451Q 3-Axis, 14-bit/8-bit Digital Accelerometer
 */
#ifndef _MMA8451Q_H_
#define _MMA8451Q_H_

#include "mbed.h"

class MMA8451Q {
public:
  /**
  * MMA8451Q constructor
  *
  * @param sda SDA pin
  * @param sdl SCL pin
  * @param addr 7bit addr of the I2C peripheral
  */
  MMA8451Q(PinName sda, PinName scl, int addr);

  /**
  * MMA8451Q destructor
  */
  ~MMA8451Q();
  
  int16_t getRawData(uint8_t addr) ;
  int16_t getRawX(void) ;
  int16_t getRawY(void) ;
  int16_t getRawZ(void) ;
  
  float getAccX(void) ;
  float getAccY(void) ;
  float getAccZ(void) ;
  
	
	void setSingleTap(void);
	void setDobleTap(void);
private:
  I2C m_i2c;
  int m_addr;
	void readRegs(int addr, uint8_t * data, int len);
  void writeRegs(uint8_t * data, int len);
} ;

#endif /* _MMA8451Q_H_ */
// by xiao_laba_cn@yahoo.com
// testing program for MLX90393
// host MCU, Arduino UNO or Atmega328p used
// read XYZ and on-chip Temperature sensor, plot as graph
// Arduino IDE 1.8.9 plot NG
// Arduino IDE 1.8.19 or 2.3.6, plot ok

#include<Wire.h>
// MLX90393 I2C Address is 0x0C(12)
#define Addr 0x0C
 
void setup()
{
    // Initialise I2C communication as MASTER
    Wire.begin();

    Serial.begin(115200);    // Initialise serial communication, set baud rate = 115200 

    // init mlx90393 register0
    Wire.beginTransmission(Addr);    // Start I2C Transmission
    Wire.write(0x60); // WR 0x60 Write Register command, register 0x00, register-0 is 16 bits
    Wire.write(0x00); // Set register-0x00 higher 8bit = 0x00, BIST disabled
    Wire.write(0x5C); // Set register-0x00 lower 8bit = 0x5C, Z-SERIES 0b0, GAIN_SEL = 0b101, HALLCONF = 0b1100
    //Wire.write(0x00<<2);  //register0x00 to be writen, shift left by 2 bits, (0x00 << 2 = 0x00, see WR commend of datasheet)
    Wire.write(0x00);  //register0x00 to be writen, shift left by 2 bits, (0x00 << 2 = 0x00, see WR commend of datasheet)    
    Wire.endTransmission();    // Stop I2C Transmission
     
    Wire.requestFrom(Addr, 1);    // Request 1 byte of data
    if(Wire.available() == 1)    // Read status byte
    {
      unsigned int c = Wire.read();
    }
     

    // init mlx90393 register2
    Wire.beginTransmission(Addr);    // Start I2C Transmission
    Wire.write(0x60);    // WR 0x60 Write Register command, register 0x00, register-0 is 16 bits
    //0x02b4 = 0b0000,0010,1011,0100 (16 bit bin)
    //           |  |  | |  | |   |
    //           000|  | |  | |   |   : reservered
    //              0 0| |  | |   |   : OSCR2
    //                 01|  | |   |   : RES_Z
    //                   0 1| |   |   : RES_Y
    //                      01|   |   : RES_X
    //                        1 01|   : DIG_FILT
    //                            00  : OSR 
    Wire.write(0x02);    // Set register-0x02, higher 8 bit
    Wire.write(0xB4);    // Set register-0x02, lower 8 bit RES for magnetic measurement = 0
    Wire.write(0x02 <<2);// register0x02 to be writen, shift left by 2 bits, (0x02 << 2 = 0x08, see WR commend of datasheet)
    Wire.endTransmission();    // Stop I2C Transmission
     
    Wire.requestFrom(Addr, 1);    // Request 1 byte of data
    if(Wire.available() == 1)    // Read status byte
    {
      unsigned int c = Wire.read();
    }
    delay(300);
    
}


/*
 * https://stackoverflow.com/questions/72059847/how-to-read-the-temperature-from-a-mlx90393-using-python
 * The temperature reading isn't in degrees; according to the datasheet, there is a scaling factor of 45.2, 
 * with an offset of 46244 at 25 degrees. So (temp - 46244) / 45.2 + 25 (and get rid of the -= 65536 part, 
 * the raw reading is unsigned). – 
 * jasonharper
 * Commented Apr 29, 2022 at 15:07
*/
// dummy routine for testing only
float convertTemperature(uint16_t rawTemp) {
  // MLX90393 temperature conversion formula // incorrect ??! not used
  // Temperature (C) = (raw_value / 8) - 75.0     
  //return (rawTemp / 8.0) - 75.0;
  return (rawTemp -46244) / 45.2 +25; // close to room temperaturem used as it is, no digging or calibration
}

// Temperature, no testing/calibration done yet, only plot for demo
void plot(float Temperature, int x, int y, int z){

    Serial.print("Temperature:"); Serial.print(Temperature); Serial.print(",");
    Serial.print("x:"); Serial.print(x); Serial.print(",");
    Serial.print("y:"); Serial.print(y); Serial.print(",");
    Serial.print("z:"); Serial.print(z); //Serial.print(",");
    Serial.print("\r\n");
}


void loop()
{
    //unsigned int data[7];
    unsigned int data[9];
     
    Wire.beginTransmission(Addr);    // Start I2C Transmission
    //single meaurement mode, command 0x30 + lower 4-bits for 4 channels as TZYX 
//  Wire.write(0x3E);    // Start single meaurement mode, ZYX enabled  
    Wire.write(0x3F);    // Start single meaurement mode, TZYX enabled
    Wire.endTransmission();    // Stop I2C Transmission

    Wire.requestFrom(Addr, 1);    // Request 1 byte of data   
    // Read status byte
    if(Wire.available() == 1)
    {
      unsigned int c = Wire.read();
    }
    delay(100);

    // measurement should be done, next step to read the result
    // Start I2C Transmission
    Wire.beginTransmission(Addr);  
//  Wire.write(0x4E); // Send read measurement command, ZYX enabled, skipped Temperature read out
    Wire.write(0x4F); // Send read measurement command, TZYX enabled
    Wire.endTransmission(); // Stop I2C Transmission
//  Wire.requestFrom(Addr, 7);  // Request 7 bytes of data, measurement ZYX
    Wire.requestFrom(Addr, 9);   // Request 9 bytes of data, measurement TZYX 

    //// Read 7 bytes of data
    //// status, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
//  if(Wire.available() == 7);

    //// Read 9 bytes of data
    //// status, T msb, T lsb, xMag msb, xMag lsb, yMag msb, yMag lsb, zMag msb, zMag lsb
    if(Wire.available() == 9);
    {
      data[0] = Wire.read();
      data[1] = Wire.read();
      data[2] = Wire.read();
      data[3] = Wire.read();
      data[4] = Wire.read();
      data[5] = Wire.read();
      data[6] = Wire.read();
      data[7] = Wire.read();
      data[8] = Wire.read();    
    }

    // Convert the data
    uint16_t rawTemp = (data[1] << 8) | data[2];
    float Temperature = convertTemperature(rawTemp);
    
    int xMag = (data[3] << 8) | data[4];
    int yMag = (data[5] << 8) | data[6];
    int zMag = (data[7] << 8) | data[8];

    plot(Temperature, xMag,yMag,zMag);  // plot data to serial plotter
    delay(100);
    
}

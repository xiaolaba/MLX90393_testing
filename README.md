# MLX90393_testing
magnometer, read on-chip temperature, X,Y,Z
no need any library, testing only  

### To read and to understand the datasheet
it was not user friendly anyway, try your best,

MLX90393 memory type, RAM (volatile memory) and ROM (perhaps FLASH memory type, non-volatile memory).

both RAM and ROM are included two sections,  
CUSTOMER AREA (the first 32 addresses are programmable by the user)  
    0x00 - 0x09 (16bit word) Analog & Digital configuration bits, register 0~9, 16bit  
    0x0A - 0x1F (16bit word) free, user can store any information  
Melexis AREA (write protected)   
    0x20 - 0x3F (16bit word) read only   

HS command, copies the entire RAM to ROM  
HR command, copies the entire ROM to RAM, it is performed automatically either through power on reset or warm reset by RT command.  

WR command, write section of CUSTOMER AREA of RAM only  
RR command, read RAM, including CUSTOMER AREA and Melexis AREA  

The customer area houses 3 types of data:  
0x00~0x09, Analog & Digital configuration bits, 16bit REGISTER x 9  
0x0A~0x1F, free, user can store any information  


example code, how to set Register-0 via I2C bus  
1, I2C write address 0x0c  
2, issue WR command  
3, value for register-0 higher-nibble  
4, value for register-0 lower-nibble  
5, register-0 << 2  
6, end I2C transimisson  
7, read STATUS byte  
```
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
```



### burn.bat
burn firmware to MCU

### wire connection  
MLX90393 pin  --- MCU pin or Arduino pin   
VCC           --- +3v3  
GND           --- GND  
SDA           --- SDA or A4  
SCL or SCK    --- SCL or A5  
![mlx90393_breadboard.JPG](mlx90393_breadboard.JPG)   

### source code
[MLX90393.ino](MLX90393.ino)  

### firmware
with bootloader for Arduino UNO/NANO  
[MLX90393.ino.with_bootloader.standard.hex  ](MLX90393.ino.with_bootloader.standard.hex)    

for Atmega328p/168/8, no bootloader used  
[MLX90393.ino.standard.hex](MLX90393.ino.standard.hex)    


### result
![testing_done.JPG](testing_done.JPG)   


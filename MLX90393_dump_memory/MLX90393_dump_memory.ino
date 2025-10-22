#include <Wire.h>

#define MLX90393_ADDRESS 0x0C

void setup() {
  Serial.begin(500000);
  //Serial.begin(115200);
  Wire.begin();
  
  Serial.println("\nMLX90393 16-bit Registers (RAM/ROM) Dump");
  Serial.println("\nBy xiao_laba_cn@yahoo.com"); 
  Serial.println("==========================================");
  
  if (!initializeMLX90393()) {
    Serial.println("Sensor initialization failed!");
    return;
  }

  Serial.println("\nSoftReset\n");
  ResetMLX();

  Serial.println("Load ROM to RAM\n");
  LoadROM();
  
  Serial.println("dump RAM, 0x00-0x1F user RAM read/write, 0x20-0x3F read only\n");
  dumpRegistersFormatted();

  Serial.println("\nClean RAM 0x00 - 0x1F, fill in 0xABCD\n");
  // clean user RAM 0x00 - 0x1F
  for (byte baseAddr = 0x00; baseAddr <= 0x1f; baseAddr++) { 
    write16bitRegister(baseAddr, 0xABCD);
  }

  Serial.println("dump RAM again, 0x00-0x1F user RAM read/write, 0x20-0x3F read only\n");
  dumpRegistersFormatted();
}

void loop() {
  // Run once
}

bool initializeMLX90393() {
  Wire.beginTransmission(MLX90393_ADDRESS);
  return (Wire.endTransmission() == 0);
}


// C-array template
// int candy[5] = {1, 2, 3, 4, 5};

void dumpRegistersFormatted() {
  //Serial.println("offset    0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F");
  //Serial.println("--------------------------------------------------------------------------------------");

  Serial.println("MLX90393 16-bit Registers (RAM/ROM) Dump to C-array");
  Serial.println("// RAM 0x00 - 0x1F, CUSTOMER AREA, READ/WRITE section");
  Serial.println("// RAM 0x20 - 0x3F, MELEXIS AREA, READ only section");
  Serial.println("//             offset  =  0       1       2       3       4       5       6       7       8       9       A       B       C       D       E       F");
//               "uint16_t REG_0x30 [16] = {0x50F7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2148, 0xFB10};"
  Serial.println("----------------------------------------------------------------------------------------------------------------------------------------------------------");




  for (byte baseAddr = 0x00; baseAddr <= 0x30; baseAddr += 0x10) {
    
    Serial.print("uint16_t REG_0x");
    if (baseAddr < 0x10) Serial.print("0");
    Serial.print(baseAddr, HEX); Serial.print(" [16] = {");

    for (byte offset = 0x00; offset < 0x10; offset++) {
      uint16_t val = read16BitRegister(baseAddr + offset);

      Serial.print("0x");
      if (val < 0x1000) Serial.print("0");
      if (val < 0x0100) Serial.print("0");
      if (val < 0x0010) Serial.print("0");
      Serial.print(val, HEX);
      
      if (offset < 0x0f) Serial.print(", ");
    }
    
    Serial.println("};");
  }
}


void LoadROM() {
  Wire.beginTransmission(MLX90393_ADDRESS);// Start I2C Transmission
  Wire.write(0xD0); // HR, load ROM to RAM
  Wire.endTransmission();    // Stop I2C Transmission

  Wire.requestFrom(MLX90393_ADDRESS, 1);    // Request 1 byte of data
  if(Wire.available() == 1)    // Read status byte
  {
    unsigned int c = Wire.read();
  }
}

void ResetMLX() {
  Wire.beginTransmission(MLX90393_ADDRESS);// Start I2C Transmission
  Wire.write(0xF0); // RT reset
  Wire.endTransmission();    // Stop I2C Transmission

  Wire.requestFrom(MLX90393_ADDRESS, 1);    // Request 1 byte of data
  if(Wire.available() == 1)    // Read status byte
  {
    unsigned int c = Wire.read();
  }
}


void write16bitRegister(byte regAddress, uint16_t value) {
  Wire.beginTransmission(MLX90393_ADDRESS);// Start I2C Transmission
  Wire.write(0x60); // WR 0x60 Write Register command
  Wire.write(value >>8); //higher 8bit
  Wire.write(value & 0xff); // lower 8bit
  Wire.write(regAddress<<2);  //register to be writen, shift left by 2 bits, << 2 = 0x00, see WR commend of datasheet)
  Wire.endTransmission();    // Stop I2C Transmission

  Wire.requestFrom(MLX90393_ADDRESS, 1);    // Request 1 byte of data
  if(Wire.available() == 1)    // Read status byte
  {
    unsigned int c = Wire.read();
  }
}


uint16_t read16BitRegister(byte regAddress) {
  Wire.beginTransmission(MLX90393_ADDRESS);
  Wire.write(0x50); // RR 0x50 Read Register command
  Wire.write(regAddress<<2);
  Wire.endTransmission();    // Stop I2C Transmission

  Wire.requestFrom(MLX90393_ADDRESS, (byte)3);
  if (Wire.available() >= 2) {
    byte status = Wire.read();
    byte highByte = Wire.read();
    byte lowByte = Wire.read();
    return (highByte << 8) | lowByte;
  } 
}

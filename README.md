# MLX90393_testing
magnometer, read on-chip temperature, X,Y,Z


### burn.bat
burn firmware to MCU

### wire connection  
MLX90393 pin  --- MCU pin or Arduino pin   
VCC           --- +3v3  
GND           --- GND  
SDA           --- SDA or A4  
SCL or SCK    --- SCL or A5  
![mlx90393_breadboard.JPG](mlx90393_breadboard.JPG)   


### firmware
with bootloader for Arduino UNO/NANO  
MLX90393.ino.with_bootloader.standard.hex    

for Atmega328p/168/8, no bootloader used  
MLX90393.ino.standard.hex    


### result
![testing_done.JPG](testing_done.JPG)   


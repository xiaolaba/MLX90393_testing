@echo off


::set mcu=atmega324p
set mcu=atmega328p

set main=MLX90393.ino.with_bootloader.standard
set ac=C:\WinAVR-20100110

path %ac%\bin;%ac%\utils\bin;%path%

avr-gcc.exe -dumpversion
::avr-gcc.exe -xc -Os -mmcu=%mcu% -Wall -g -o %main%.out *.c
::avr-gcc.exe -xc -Os -mmcu=%mcu% -DF_CPU=16000000UL -Wall -g -o %main%.out *.c


::avr-gcc.exe -O2 -Wl,-Map,%1.map -o %1.out %1.c %2 %3 -mmcu=at%mcu%
::cmd /c avr-objdump.exe -h -S %main%.out >%main%.lst
::cmd /c avr-objcopy.exe -O ihex %main%.out %main%_WinAVR-20100110.hex
::avr-size.exe %main%.out
::del %main%.out


:: burn the firmware
avrdude -c usbtiny -p m328p -U flash:w:"%main%.hex":a
:end
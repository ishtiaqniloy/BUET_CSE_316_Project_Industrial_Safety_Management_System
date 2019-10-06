avr-gcc -g -Os -mmcu=atmega32 -c Test.c
avr-gcc -g -mmcu=atmega32 -o Test.elf Test.o
avr-objcopy -j .text -j .data -O ihex Test.elf Test.hex
avr-size --format=avr --mcu=atmega32 Test.elf

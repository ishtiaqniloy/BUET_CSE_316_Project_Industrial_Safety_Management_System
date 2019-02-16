avrdude -c usbasp -p m32 -P /dev/ttyACM0 -B10 -U flash:w:Test.hex -F

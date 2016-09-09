# Multi_IRR-IRS
This is a simple project to deal with many IR receivers or emitters with only one arduino board.
Sensors' data-pins are supposed to be connected to the arduino through multiplexer. The muxer is a 2^nto1, so only one wire connects it to the arduino.
Tests have revealed that it's more reliable to maintain sensor at HIGH level and not to switch using arduino.
Please refer to the attatched schematics for more details.

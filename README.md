# Multi_IRR-IRS
This program is designed to autodrive a robot to precise direction ( parking, docking station, etc).
ROS middleware is required to achieve the mission, elsecase you may change some parts of the code.
Check if the ROS and IRremote libraries are preintalled, if not you can download them..
This code supposes that the robot is connected to an IR/US sensor within the topic /IR3 and that two external IRemitters are active and converging.
When connecting components Please refer to the schematics 

To run autopilot execute these commands:
1 $ roscore
2 $ rosrun rosserial_python serial_node.py /dev/ttyACM<ArduinoFileNum> __name:=autopilot

Sensors' data-pins are supposed to be connected to the arduino through multiplexer. The muxer is a 2^nto1, so only one wire connects it to the arduino.
Tests have revealed that it's more reliable to maintain sensor at HIGH level and not to switch using arduino.
Please refer to the attatched schematics for more details.

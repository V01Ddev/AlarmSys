#!/bin/bash

MC="/dev/ttyACM0" # Port to be used 
FN="MotionSense.ino" # filename

arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso $FN

arduino-cli upload -p $MC --fqbn esp32:esp32:esp32-poe-iso $FN

putty $MC -serial -sercfg 115200

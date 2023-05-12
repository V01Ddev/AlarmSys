#!/bin/bash

MC="/dev/ttyUSB0"
FN="LEDAlarm.ino"

arduino-cli compile --fqbn esp32:esp32:esp32-poe-iso $FN

arduino-cli upload -p $MC --fqbn esp32:esp32:esp32-poe-iso $FN

putty $MC -serial -sercfg 115200

# btadc
Raspberry pi - Interface to store continuously streamed data from an arduino 

Tested with ESP32Wroom and Raspberry Pi 4.

ADC pins are 34 and 35. Upload and provide power to the arduino. It will setup BT and wait for a byte to come in before streaming as many samples as specified in the PI file.

The Pi needs to be configured to use bluetooth as its comm port. 
Follow these instructions: https://forums.raspberrypi.com/viewtopic.php?p=947185

After that, pair the devices and run python3.7 btadc.py [path] [samples]

where path is the name/destination of the output csv file and samples is the number of samples you want to take. 



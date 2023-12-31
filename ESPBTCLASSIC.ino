#include "BluetoothSerial.h"
#include "driver/adc.h"
#include <AccelStepper.h>


#define SAMPLESPERSTEP 1000 //Set this to whatever you want 
#define p1 19
#define p2 16
#define p3 17
#define p4 21

AccelStepper stepper = AccelStepper(AccelStepper::FULL4WIRE, p1,p2,p3,p4); //bottom left 4 pins. You can change with define statements above

String device_name = "Magnometer";
int reading, counter = 0;
volatile int j = -1024;
volatile char hisend, losend;
volatile byte buff[2];
bool acknowledged = false;

adc1_channel_t channelHi = ADC1_CHANNEL_6;
adc1_channel_t channelLo = ADC1_CHANNEL_7;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  adc1_config_width(ADC_WIDTH_BIT_12);
  Serial.begin(115200);
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("\"%s\" is started.", device_name.c_str());
  #ifdef USE_PIN
    SerialBT.setPin(pin);
    Serial.println("Using PIN");
  #endif
}

void loop() {
  //reading = int(adc1_get_raw(channelHi)-adc1_get_raw(channelLo));
  if(SerialBT.available())
  {
    if (SerialBT.read() == 49)
    {
    acknowledged = !acknowledged;
    counter = 0;
    }
    Serial.println(SerialBT.read());
    
  }
  if (acknowledged)
  {
  reading = int(adc1_get_raw(channelHi)-adc1_get_raw(channelLo));
  
  buff[0] = *((uint8_t*)&(reading)+1);
  buff[1] = *((uint8_t*)&(reading)+0);

  SerialBT.write(buff[0]);
  SerialBT.write(buff[1]);
  counter++;
  if (counter % SAMPLESPERSTEP == 0) 
  {
    stepper.move(1);
    //stepper.runToPosition();
    //Serial.println("IN");
    stepper.run();
  } 
}
}

/*
  DMX Blink
  This sketch toggles the value of DMX channel 1 between 255 and 0.
  Circuit:
   - DMX light
   - MKR board
   - MKR 485 shield
     - ISO GND connected to DMX light GND (pin 1)
     - Y connected to DMX light Data + (pin 2)
     - Z connected to DMX light Data - (pin 3)
     - Jumper positions
       - Z \/\/ Y set to ON
  created 5 July 2018
  by Sandeep Mistry
*/

#include <ArduinoRS485.h> // the ArduinoDMX library depends on ArduinoRS485
#include <ArduinoDMX.h>

const int universeSize = 16;

int num_Measure = 128 ; // Set the number of measurements   
int pinSignal = A0; // pin connected to pin O module sound sensor   
long Sound_signal;    // Store the value read Sound Sensor   
long sum = 0 ; // Store the total value of n measurements   
long level = 0 ; // Store the average value
long maxLevel = 550;
long minLevel = 35;
int sensitvity = 2;

int lights[16][3] = {0};

void printLights()
{
  // We have 16 rows/columns of lights and each has RGB values
  // A new level should rise every 22 levels or so
  //long oneLevel = (maxLevel - 35) / 16;

  // By giving the smallest possible value of the noise level and it's maximum,
  // calculate how many lights should turn on. Values from 0 - 16
  int amount = map(level, minLevel, maxLevel, 0, 16);
  
  DMX.beginTransmission();
  for(int i = 0; i < amount; i++){
    lights[i][1] = 255;
    DMX.write(i, 255);
    DMX.write(i + 1, 255);
    DMX.write(i + 2, 100);
  }
  for(int i = 15; i > amount - 1; i--){
    lights[i][1] = 0;
    DMX.write(i, 0);
    DMX.write(i + 1, 0);
    DMX.write(i + 2, 0);
  }

  DMX.endTransmission();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // initialize the DMX library with the universe size
  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1); // wait for ever
  }
}

void loop() {
  // set channel 1 value to 255

// Performs 128 signal readings   
  for (int i = 0 ; i < num_Measure; i ++)  
  {  
   Sound_signal = analogRead (pinSignal);  
    sum =sum + Sound_signal;  
  }  

  level = min(sum / num_Measure, maxLevel); // Calculate the average value and compare to maxLevel and take the smaller
  Serial.print("Sound Level: ");
  Serial.println (level);  
  printLights();
  sum = 0 ; // Reset the sum of the measurement values

  delay(100);
}

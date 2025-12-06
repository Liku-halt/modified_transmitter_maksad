#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#define s0 16
#define s1 5
#define CE 2
#define CSN 15
#define analog A0
//making an object
RF24 hydra(CE,CSN);
//first address 0th and the second address is 1st.
uint8_t address[][6]= {"rain0","fall0"};
bool role;
byte reading;
int calibration=21;


struct datasets {
    byte x1;
    byte y1;
    byte x2;
    byte y2;
};

datasets data;

void setup() {
    Serial.begin(115200);
    calibration=21;
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(A0, INPUT);
  while(!hydra.begin()){
    Serial.println("hardware is not responding");
    delay(20);
  }
  hydra.setPALevel(RF24_PA_LOW);
  hydra.setPayloadSize(sizeof(datasets));
  hydra.openWritingPipe(address[0]);
  hydra.setChannel(10);
  hydra.printPrettyDetails();
  //calibration loop
  digitalWrite(s0, 0);
  digitalWrite(s1, 0);
  delay(10);
  while(calibration>20){
    digitalWrite(s0, 1);
    digitalWrite(s1, 0);
    delay(20);
    calibration= map(analogRead(analog),0,1024,0,250);
    Serial.println(calibration);
  }
  Serial.print("calibration value for j1 ;");
  Serial.println(calibration);
  delay(500);
}

void loop() {
    // Read channel 0
    digitalWrite(s0, 0);
    digitalWrite(s1, 0);
    //delay(1);  // Stabilization time
    data.x1 = map(analogRead(analog),0,1024,calibration,245);

    // Read channel 1
    digitalWrite(s0, 1);
    digitalWrite(s1, 0);
   // delay(1);
    data.y1 = map(analogRead(analog),0,1024,calibration,245);

    // Read channel 2
    digitalWrite(s0, 0);
    digitalWrite(s1, 1);
    //delay(1);
    data.x2 = analogRead(analog);


    // Read channel 3
    digitalWrite(s0, 1);
    digitalWrite(s1, 1);
    //delay(1);
    data.y2 = map(analogRead(analog),0,1024,9,245);
    delay(2);
    transmit();
}
void transmit(){
    hydra.write(&data,sizeof(data));
  

}


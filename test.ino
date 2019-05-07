#include <TimerOne.h>

int index = 0;
int wI = 0;
int rI = 0;
int delta = 1;
int arr[] = {127, 160, 190, 217, 237, 250, 254, 250, 237, 217, 191, 160, 127, 94, 63, 36, 16, 3, 0, 3, 15, 36, 62, 93, 126};
int circBuffer[256]; 

void setup() {
    
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    Timer1.initialize(1000000); 
    Timer1.attachInterrupt( timerIsr );
}

void loop() {
    // Serial.print(circBuffer[wI]);
    // Serial.print(" ");
    Serial.println(circBuffer[rI + delta]);
    rI++;
    if((rI + delta) >= 255){
        rI = 0;
    }
    delayMicroseconds(2000000);
}
void timerIsr(){
    circBuffer[wI] = arr[index];
    
    wI++;
    index++;
    if(index == 24){
        index = 0;
    }
    if(wI == 255){
        wI == 0;
    }
}

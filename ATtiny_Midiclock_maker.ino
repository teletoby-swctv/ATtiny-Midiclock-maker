#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX


byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = 0;
byte data;
byte counter;
byte led1 = 5;           //LED1 D5 - pin8
byte led2 = 4;           //LED2 D4 - pin9
byte led3 = 3;           //LED3 D3 - pin10
byte led4 = 2;           //LED4 D2 - pin11
byte sync_int_pin = 7;   //bpm in/ext - D7 pin6
byte ledIR = 8;        //IR led sync out - D8 pin5
byte sync_int = 0;
byte bpm_pot = A6;       //poti BPM  - A6 pin7
int bpm_int;
unsigned long bpm_time;
unsigned long previousMillis = 0;
void setup() {
mySerial.begin(31250);
pinMode(led1, OUTPUT);
pinMode(led2, OUTPUT);
pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
pinMode(sync_int_pin, INPUT);
pinMode(bpm_pot, INPUT);
pinMode(ledIR, OUTPUT);

digitalWrite(led1, HIGH);
delay(50);
digitalWrite(led2, HIGH);
delay(50);
digitalWrite(led3, HIGH);
delay(50);
digitalWrite(led4, HIGH);

}

void loop() {
sync_int = digitalRead(sync_int_pin);

if(sync_int==0){
  //digitalWrite(led1, LOW);
  //digitalWrite(led2, LOW);
  //digitalWrite(led3, LOW);
  //digitalWrite(led4, LOW);
 // bpm_int=analogRead(bpm_pot);
  bpm_int=(map(analogRead(bpm_pot),0,1024,10,250)); //http://hofa-college.de/online_campus/pages/tipps-amp-tricks/hofa-tools/bpm-in-msec.php
  bpm_time=2500000/bpm_int;     //bpm_time=(60*1000*1000/24)/bpm_int;  

  unsigned long currentMillis = micros();
 
  if(currentMillis - previousMillis > bpm_time) {            // save the last time you blinked the LED 
    digitalWrite(ledIR, HIGH);
    Sync();
    previousMillis = currentMillis;
    //IRblink();
    digitalWrite(ledIR, LOW);  
  }
}

if(sync_int==1){
 if(mySerial.available() > 0) {
  data = mySerial.read();
 if(data == midi_start) {
  play_flag = 1;
  
 }
 else if(data == midi_continue) {
  play_flag = 1;
 }
 else if(data == midi_stop) {
  play_flag = 0;
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
}
    else if(data == midi_clock) {    //((data == midi_clock) && (play_flag == 1)) 
    Sync();
    }
  }
 }
}

void Sync() {        // do something for every MIDI Clock pulse when the sequencer is running
mySerial.write(midi_clock);

if(counter == 0){
 //turnLedOn();
 digitalWrite(led4, LOW);
 digitalWrite(led1, HIGH);
 IRblink();
 }

if(counter == 6){
digitalWrite(led1, LOW);
digitalWrite(led2, HIGH);
IRblink();
}

if(counter == 12){
 digitalWrite(led2, LOW);
 digitalWrite(led3, HIGH);
 IRblink();
 }

if(counter == 18){
 digitalWrite(led3, LOW);
 digitalWrite(led4, HIGH);
 IRblink();
 }
   
counter++;
counter%=24;

} 

void IRblink(){
  digitalWrite(ledIR, HIGH);
  delay(1);
  digitalWrite(ledIR, LOW);
}

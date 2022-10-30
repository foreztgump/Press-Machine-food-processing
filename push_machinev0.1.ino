#include "BTS7960.h"

#define current_sensor A5

const int button_start = 3;   
const int button_stop = 4;   
const unsigned long debounceTime = 10;  // milliseconds
const int ledPin =  2;     
const uint8_t EN = 8;
const uint8_t L_PWM = 10;
const uint8_t R_PWM = 9;
BTS7960 motorController(EN, L_PWM, R_PWM);


//right = extend
//left = retract

int flag_working = 0;
int flag_stop = 0;
byte button_start_state = LOW;
byte button_stop_state = LOW;
byte old_start_state = HIGH;
byte old_stop_state = HIGH;
unsigned long switchPressTime;  // when the switch last changed state


void setup() {
  Serial.begin(9600);  
  for(int i=11; i < 22; i++) {
    if(i ==  18){
      //skip
    }
    else if (i == 19){
      //skip
    }
    else {
      digitalWrite(i, LOW);
    }
  }
  
  pinMode(button_start, INPUT_PULLUP);
  pinMode(button_stop, INPUT_PULLUP);

  motorController.Enable();
}

void loop() {
  checkStartButton();
  if (flag_working == 1){
    pushWorking();
  }
  delay(5);
}

void checkStartButton() {
  button_start_state = digitalRead(button_start);
  if (button_start_state != old_start_state) {
    if (millis () - switchPressTime >= debounceTime) {
       switchPressTime = millis ();  // when we closed the switch 
       old_start_state =  button_start_state;  // remember for next time 
       if (button_start_state == LOW) {
          if (flag_working == 1) {
            flag_working = 0;
          }
          else {
            flag_working = 1;
          }
       }
    }
  }  
}

void checkStopButton() {
  button_stop_state = digitalRead(button_stop);
  if (button_stop_state != old_stop_state) {
    if (millis () - switchPressTime >= debounceTime) {
       switchPressTime = millis ();  // when we closed the switch 
       old_stop_state =  button_stop_state;  // remember for next time 
       if (button_stop_state == LOW) {
          flag_stop = 1;
       }
    }
  }  
}

int currentMonitor() {
  unsigned int x=0;
  float AcsValue=0.0, Samples=0.0, AvgAcs=0.0, AcsValueF=0.0;

  for (int x = 0; x < 150; x++){
    AcsValue = analogRead(current_sensor);
    Samples = Samples + AcsValue;
    delay(3);
  }
  AvgAcs=Samples/150.0;
  Serial.println(AvgAcs);
  return AvgAcs;
}

void pushWorking() {
  push();
  Serial.println("Started");
  while(1) {
    //
    float current = 0.0;
    current = currentMonitor();
    checkStartButton();
    checkStopButton();
    if (flag_stop == 1) {
      motorController.Stop();
      Serial.println("Stop");
      flag_stop = 0;
    }
    if (flag_working == 0) {
      pull();
      Serial.println("Retract");
      break;
    }
    if (current >= 760.00) {
      digitalWrite(ledPin, LOW);
      motorController.Stop();
      delay(8000);
      flag_working = 0;
    }
  }
}

void push() {
  digitalWrite(ledPin, HIGH);
  for(int speed = 0 ; speed < 255; speed+=40) {
    motorController.TurnRight(speed); 
    delay(100);
  }  
}

void pull() {
  digitalWrite(ledPin, LOW);
  for(int speed = 0 ; speed < 255; speed+=40) {
    motorController.TurnLeft(speed);
    delay(100);
  }  
}

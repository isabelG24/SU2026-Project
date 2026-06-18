#include "CNCShield.h"
#include <Stepper.h>
#include "SimpleServo.h"

#define STEPS 2048

//Joystick
const int xPin = A10;  // VRX pin attached to A0
const int yPin = A11;  // VRY pin attached to A1
int xValue;            // X value
int yValue;            // Y value


//cnc shield
const int enPin = 8;
//x direction
const int stepXPin = 2;  //X.STEP
const int dirXPin = 5;   // X.DIR
//y direction
const int stepZPin = 4;  //Z.STEP
const int dirZPin = 7;   // Z.DIR

CNCShield myCNC(enPin, stepXPin, dirXPin, stepZPin, dirZPin);

// Limit switches
// X axes
const int limitXminus = 9;
const int limitXplus = 10;
bool limitXminusPressed;
bool limitXplusPressed;

// X axes
const int limitYminus = 48 ;
const int limitYplus = 46;
bool limitYminusPressed;
bool limitYplusPressed;

// Stepper motor 
//                    in1 in3 in2 in4
Stepper stepper(STEPS, 29, 31, 33, 35);

//Button for claw
int buttonPin = 41;
bool buttonPressed;

// servo
SimpleServo myServo(24);  // connect servo signal to pin 24

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // Start serial communication at 9600 baud rate
  
  myCNC.setModeCNC();

  //endswitch
  pinMode(limitXminus, INPUT_PULLUP);
  pinMode(limitXplus, INPUT_PULLUP);
  pinMode(limitYminus, INPUT_PULLUP);
  pinMode(limitYplus, INPUT_PULLUP);

  //Stepper Motor(Claw)
  stepper.setSpeed(15);
  pinMode( buttonPin, INPUT_PULLUP);

  //servo
  myServo.begin();
  myServo.write(180); 
  delay(1000);

}

void loop() {

  xValue = analogRead(xPin); 
  yValue = analogRead(yPin);

  limitXminusPressed = (digitalRead(limitXminus) == LOW); // True when switch is pressed
  limitXplusPressed = (digitalRead(limitXplus) == LOW);

  limitYminusPressed = (digitalRead(limitYminus) == LOW); // True when switch is pressed
  limitYplusPressed = (digitalRead(limitYplus) == LOW);

  buttonPressed = (digitalRead(buttonPin) == LOW);

  if(buttonPressed){
    //Print when the switch is pressed
    Serial.println("button pressed claw going down");
    delay(1000);
    myServo.write(20);//back to 0 degrees
    delay(1000);
    stepper.step(STEPS); 
    //delay(7000);
    stepper.step(STEPS); 
    stepper.step(STEPS);
    stepper.step(STEPS);
    delay(1000);
    myServo.write(180);//back to 0 degrees
    delay(1000);
    //delay(7000);
    stepper.step(-STEPS); 
    //delay(7000);
    stepper.step(-STEPS); 
    stepper.step(-STEPS);
    stepper.step(-STEPS);
    delay(1000);
    while(!(digitalRead(limitXplus) == LOW)){
      myCNC.moveRight();
    }
    while(!(digitalRead(limitYplus) == LOW)){
      myCNC.moveForward();
    }
    myServo.write(20);
    delay(1000);
    myServo.write(180);
  }


#include <Wire.h>

// Main right motor (back)
const int AIN1 = 13;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 12;            //control pin 2 on the motor driver for the right motor
const int PWMA = 11;            //speed control pin on the motor driver for the right motor

// Secondary right motor (front)
const int AIN4 = 7;
const int AIN3 = 4;
const int PWMC = 5;

// Main left motor (back)
const int PWMB = 10;           //speed control pin on the motor driver for the left motor
const int BIN2 = 9;           //control pin 2 on the motor driver for the left motor
const int BIN1 = 8;           //control pin 1 on the motor driver for the left motor

// Secondary left motor (front)
const int PWMD = 6;
const int BIN4 = 3;
const int BIN3 = 2;

//distance variables
const int trigPin = 8;
const int echoPin = 9;

float distance = 0;            //variable to store the distance measured by the distance sensor

//robot behaviour variables
int backupTime = 300;           //amount of time that the robot will back up when it senses an object
int turnTime = 200;             //amount that the robot will turn once it has backed up

/********************************************************************************/
void setup()
{
  
  pinMode(trigPin, OUTPUT);       //this pin will send ultrasonic pulses out from the distance sensor
  pinMode(echoPin, INPUT);        //this pin will sense when the pulses reflect back to the distance sensor


  // Main right motor (back)
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  // Secondary right motor (front)
  pinMode(AIN4, OUTPUT);
  pinMode(AIN3, OUTPUT);
  pinMode(PWMC, OUTPUT);

  // Main left motor (back)
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  
  // Secondary left motor (front)
  pinMode(BIN3, OUTPUT);
  pinMode(BIN4, OUTPUT);
  pinMode(PWMD, OUTPUT);

  Serial.begin(9600);                       //begin serial communication with the computer
  Serial.print("To infinity and beyond!");  //test the serial connection

}

/********************************************************************************/
void loop()
{
  //DETECT THE DISTANCE READ BY THE DISTANCE SENSOR
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" in");              // print the units

  

  if (distance < 10) {              //if an object is detected
    //back up and turn
    Serial.print(" ");
    Serial.print("BACK!");

    //stop for a moment
    rightmotorBack(0);
    rightmotorFront(0);
    leftmotorBack(0);
    leftmotorFront(0);
    delay(200);

    //back up
    rightmotorBack(255);
    rightmotorFront(255);
    leftmotorBack(-255);
    leftmotorFront(-255);
    delay(backupTime);

    //turn away from obstacle
    rightmotorBack(-255);
    rightmotorFront(-255);
    leftmotorBack(-255);
    leftmotorFront(-255);
    delay(turnTime);

  } 
    
  else {                        //if no obstacle is detected drive forward
    Serial.print(" ");
    Serial.print("Moving...");

    rightmotorBack(-255);
    rightmotorFront(-255);
    leftmotorBack(255);
    leftmotorFront(255);     
  }
    
delay(50);                      //wait 50 milliseconds between readings
}

/********************************************************************************/

// Main (Back) Right Motor 
void rightmotorBack(int motorSpeed){                       //function for driving the right motor
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}


// Secondary (Front) Right Motor
void rightmotorFront(int motorSpeed){                       //function for driving the right motor
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN4, HIGH);                         //set pin 1 to high
    digitalWrite(AIN3, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN4, LOW);                          //set pin 1 to low
    digitalWrite(AIN3, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN4, LOW);                          //set pin 1 to low
    digitalWrite(AIN3, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMC, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}


/********************************************************************************/
// Main (Back) Left Motor 
void leftmotorBack(int motorSpeed){                        //function for driving the left motor

  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}


// Secondary (Front) Left Motor 
void leftmotorFront(int motorSpeed){                        //function for driving the left motor

  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN3, HIGH);                         //set pin 1 to high
    digitalWrite(BIN4, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN3, LOW);                          //set pin 1 to low
    digitalWrite(BIN4, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN3, LOW);                          //set pin 1 to low
    digitalWrite(BIN4, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMD, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}
/********************************************************************************/

//RETURNS THE DISTANCE MEASURED BY THE HC-SR04 DISTANCE SENSOR
float getDistance()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

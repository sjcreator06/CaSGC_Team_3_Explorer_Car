
//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 7;           //control pin 1 on the motor driver for the right motor  (13)
const int AIN2 = 6;            //control pin 2 on the motor driver for the right motor  (12)
const int PWMA = 5;            //speed control pin on the motor driver for the right motor  (11)

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 4;           //speed control pin on the motor driver for the left motor  (10)
const int BIN2 =3;           //control pin 2 on the motor driver for the left motor  (9)
const int BIN1 = 2;           //control pin 1 on the motor driver for the left motor  (8)

float distanceTop = 0;            //variable to store the distance measured by the top distance sensor
float distanceBottom = 0;         //variable to store the distance measured by the bottom distance sensor


//robot behaviour variables
int backupTime = 300;           //amount of time that the robot will back up when it senses an object
int turnTime = 200;             //amount that the robot will turn once it has backed up

/********************************************************************************/
void setup()
{
  // Top Distance Sensor
  pinMode(A4, OUTPUT);       //this pin will send ultrasonic pulses out from the distance sensor
  pinMode(A5, INPUT);        //this pin will sense when the pulses reflect back to the distance sensor


  // Bottom Distance Sensor
  pinMode(A1, OUTPUT);   //the trigger pin will output pulses of electricity
  pinMode(A2, INPUT);    //the echo pin will measure the duration of pulses coming back from the distance sensor

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.begin(9600);                       //begin serial communication with the computer
  Serial.println("To infinity and beyond!");  //test the serial connection

}

/********************************************************************************/
void loop()
{
  //DETECT THE DISTANCE READ BY THE TOP DISTANCE SENSOR
  distanceTop = getDistanceTop();

  Serial.print("Top Distance: ");
  Serial.print(distanceTop);
  Serial.println(" in");              // print the units


  //DETECT THE DISTANCE READ BY THE BOTTOM DISTANCE SENSOR
  distanceBottom = getDistanceBottom();

  Serial.print("Bottom Distance: ");
  Serial.print(distanceBottom);
  Serial.println(" in");              // print the units
  Serial.print("---------------------------------------------");

  


    if (distanceTop < 7 or distanceBottom < 10) {              //if an object is detected
      //back up and turn
      Serial.println(" ");
      Serial.println("BACK!");
      
      //stop for a moment
      rightMotor(0);
      leftMotor(0);
      delay(800);

      //back up
      rightMotor(-255);
      leftMotor(255);
      delay(800);

      //turn away from obstacle
      rightMotor(255);
      leftMotor(0);
      delay(1400);
      

    } else {                        //if no obstacle is detected drive forward
      Serial.println(" ");
      Serial.println("Moving...");

      rightMotor(255);
      leftMotor(-255);
      
    }

  
  
}

/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
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

/********************************************************************************/
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
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

/********************************************************************************/
//RETURNS THE DISTANCE MEASURED BY THE TOP DISTANCE SENSOR
float getDistanceTop()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(A4, HIGH);
  delayMicroseconds(10);
  digitalWrite(A4, LOW);

  echoTime = pulseIn(A5, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}


//RETURNS THE DISTANCE MEASURED BY THE BOTTOM DISTANCE SENSOR
float getDistanceBottom()
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(A1, HIGH);
  delayMicroseconds(10);
  digitalWrite(A1, LOW);

  echoTime = pulseIn(A2, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor

  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)

  return calculatedDistance;              //send back the distance that was calculated
}

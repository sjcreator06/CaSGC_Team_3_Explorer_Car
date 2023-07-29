
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

const int driveTime = 20;      //this is the number of milliseconds that it takes the robot to drive 1 inch
                               //it is set so that if you tell the robot to drive forward 25 units, the robot drives about 25 inches

const int turnTime = 8;        //this is the number of milliseconds that it takes to turn the robot 1 degree
                               //it is set so that if you tell the robot to turn right 90 units, the robot turns about 90 degrees

                               //Note: these numbers will vary a little bit based on how you mount your motors, the friction of the
                               //surface that your driving on, and fluctuations in the power to the motors.
                               //You can change the driveTime and turnTime to make them more accurate

String botDirection;           //the direction that the robot will drive in (this change which direction the two motors spin in)
String distance;               //the distance to travel in each direction

/********************************************************************************/
void setup(){
  //set the motor control pins as outputs
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

  Serial.begin(9600);           //begin serial communication with the computer

  //prompt the user to enter a command
  Serial.println("Enter a direction followed by a distance.");
  Serial.println("f = forward, b = backward, r = turn right, l = turn left");
  Serial.println("Example command: f 50");
}

/********************************************************************************/
void loop(){

  if (Serial.available() > 0){                         //if the user has sent a command to the RedBoard
    botDirection = Serial.readStringUntil(' ');       //read the characters in the command until you reach the first space
    distance = Serial.readStringUntil(' ');           //read the characters in the command until you reach the second space

    //print the command that was just received in the serial monitor
    Serial.print(botDirection);
    Serial.print(" ");
    Serial.println(distance.toInt());

    if (botDirection == "f")                         //if the entered direction is forward
    {
      rightmotorBack(-200);                                //drive the right wheel forward
      leftmotorBack(200);                                 //drive the left wheel forward
      rightmotorFront(-200);                                //drive the right wheel forward
      leftmotorFront(200); 
      delay(driveTime * distance.toInt());            //drive the motors long enough travel the entered distance
      rightmotorBack(0);                                  //turn the right motor off
      leftmotorBack(0);                                   //turn the left motor off
      rightmotorFront(0);                                
      leftmotorFront(0); 
    }
    else if (botDirection == "b")                    //if the entered direction is backward
    {
      rightmotorBack(200);                               //drive the right wheel forward
      leftmotorBack(-200);                                //drive the left wheel forward
      rightmotorFront(200);                                
      leftmotorFront(-200); 
      delay(driveTime * distance.toInt());            //drive the motors long enough travel the entered distance
      rightmotorBack(0);                                  //turn the right motor off
      leftmotorBack(0);                                   //turn the left motor off
      rightmotorFront(0);                                
      leftmotorFront(0); 
    }
    else if (botDirection == "r")                     //if the entered direction is right
    {
      rightmotorBack(200);                               //drive the right wheel forward
      leftmotorBack(255);                                 //drive the left wheel forward
      rightmotorFront(200);                                
      leftmotorFront(255); 
      delay(turnTime * distance.toInt());             //drive the motors long enough turn the entered distance
      rightmotorBack(0);                                  //turn the right motor off
      leftmotorBack(0);                                   //turn the left motor off
      rightmotorFront(0);                                
      leftmotorFront(0); 
    }
    else if (botDirection == "l")                   //if the entered direction is left
    {
      rightmotorBack(-255);                                //drive the right wheel forward
      leftmotorBack(-200);                                //drive the left wheel forward
      rightmotorFront(-255);                                
      leftmotorFront(-200); 
      delay(turnTime * distance.toInt());             //drive the motors long enough turn the entered distance
      rightmotorBack(0);                                  //turn the right motor off
      leftmotorBack(0);                                   //turn the left motor off
      rightmotorFront(0);                                
      leftmotorFront(0); 
    }
  }
  
  else
  {
    rightmotorBack(0);                                  //turn the right motor off
    leftmotorBack(0);                                   //turn the left motor off
    rightmotorFront(0);                                
    leftmotorFront(0); 
    
  }
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

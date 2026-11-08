/*
 Honor Pledge: I pledge my honor that I have abided by the Stevens Honor System.
*/

//Import necessary libraries
#include <SSD1306Wire.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <Servo.h>

// Creating obects for both motors, dispaly, and sensors
Servo motor_left;
Servo motor_right;
SSD1306Wire display(0x3C, D2, D1);
Ultrasonic ultrasonic_front(D5, D8);
Ultrasonic ultrasonic_right(D4, D7);
Ultrasonic ultrasonic_left(TX, D0);

// Initializing data variables for use in program
const int STOP_DISTANCE_CM = 18;
const int RIGHT_DISTANCE_CM = 9;

// Stop robot function to stop both motors
void stopRobot() {
  motor_left.write(90);
  motor_right.write(90);
}

//turn around function, moves motors in opposite directions to turn the robot around in place, 500 ms delay to ensure motor innstruction execution
void turnAround(){
  motor_left.write(70);
  motor_right.write(110);
  delay(1500);
  motor_left.write(90);
  motor_right.write(90);
  delay(40);

}

void turnLeft(){
  motor_left.write(70);
  motor_right.write(110);
  delay(720);
  motor_left.write(90);
  motor_right.write(90);
  delay(40);
}

void leftCorrection(){
  motor_left.write(120);
  motor_right.write(123);
  delay(20);
}

void rightCorrection(){
  motor_left.write(123);
  motor_right.write(120);
  delay(20);
}

// Intializing setup for the robot
void setup() {
  //Serial.begin(115200);
  motor_left.attach(D3);
  motor_right.attach(D6);
  motor_left.write(90);
  motor_right.write(90);
  display.init();
  display.flipScreenVertically();
  delay(300);

}

void loop() {

     int dist_cm_front = ultrasonic_front.read(CM);
     int dist_cm_right = ultrasonic_right.read(CM);
     int dist_cm_left = ultrasonic_left.read(CM);
     delay(20);

// If distance < stop_distance turn the robot around (prepare to traverse back to start)
    
    if (dist_cm_front <= STOP_DISTANCE_CM ) {
      //move robot forward until wall detected
     stopRobot();
     delay(20);
     turnLeft();
    } 
    else if(dist_cm_right < RIGHT_DISTANCE_CM){
      leftCorrection();
    }
    else if(dist_cm_right > RIGHT_DISTANCE_CM){
      rightCorrection();
    }
    else {
      //move robot forward until wall detected
     motor_left.write(121);
     motor_right.write(120);
     delay(20);
    }

// Pushing headers to OLED Display
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "Front: ");
    display.drawString(0, 18, "Right: ");
    display.drawString(0, 36, "Left: ");

 
    // Publishing sensor data to OLED Display
    display.drawString(90, 0, String(dist_cm_front));
    display.drawString(90, 18, String(dist_cm_right));
    display.drawString(90, 36, String(dist_cm_left));

    display.display();
    delay(20);
    display.clear();
}

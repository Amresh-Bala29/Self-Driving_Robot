/*
Pledge: I pledge my Honor that I have abided by the Stevens Honor System. 
*/

// Libraries to include
#include <SSD1306Wire.h>
#include <Wire.h>
#include <Ultrasonic.h>
#include <Servo.h>

//Creating objects for motors,sensors, and display
Servo motor_left;
Servo motor_right;
SSD1306Wire display(0x3C, D2, D1);
Ultrasonic ultrasonic_front(D5, D8);

// stop distance ~ 9 inches
const int STOP_DISTANCE_CM = 18;

//Start data when starting robot
int data = 180;
int dist_cm = 400;

// Stop Robot function
void stopRobot() {
  motor_left.write(90);
  motor_right.write(90);
}

//Setup code, runs when system start
void setup() {
  Serial.begin(115200);
  motor_left.attach(D6);
  motor_right.attach(D3);
  display.init();
  display.flipScreenVertically();

}

void loop() {

 // Detects wall, if the wall not detected robot continues moving straight forward
    if (dist_cm <= STOP_DISTANCE_CM) {
      stopRobot();
    } else {
      // Robot moves forward if dist_cm > stop distance
      motor_left.write(data);
      motor_right.write(data);
    }

 
// Casting data to string to print to OLED Display
    char distbuf[50];
    sprintf(distbuf, "%d", dist_cm);
    const char *d = distbuf;

    char lbuf[50];
    sprintf(lbuf, "%d", data);
    const char *l = lbuf;

    char rbuf[50];
    sprintf(rbuf, "%d", data);
    const char *r = rbuf;

// Displaying data to the OLED display
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "L Motor:");
    display.drawString(0, 18, "R Motor:");
    display.drawString(0, 36, "Dist(cm):");

    display.drawString(90, 0, l);
    display.drawString(90, 18, r);
    display.drawString(90, 36, d);

// Clears display
    display.display();
    display.clear();

// Delay in between each iteration
  delay(120);
}

/*========================================================================================================================
 * Ver.1 – 03/02/2026 SES IDEAs Program 
 * 2026S ENGR122 Sample Code for Week 7 Activity & Final Project 
 * Camera Sensor–Based Position Feedback using MQTT Network
 *
 * This sketch is provided as a baseline for the Week 7 lab activity and Final Project
 * Week7 Activity:
 *  -Based on this baseline code, students must complete the program to display the robot's position on the OLED display.
 * Final  Project:
 *  -Using the ultrasonic sensor, students must design an Obstacle Avoidance logic.
 *  -Using camera-sensor data received through the MQTT network, students must implement a Path-Planning algorithm.
 *  -Students must integrate these algorithms to control the two motors so that the robot can reach designated targets. 
 *  -The OLED display should be used for debugging purposes.
 *=========================================================================================================================*/

 //Names: Amresh, Ava, Abuzer
 //Pledge: We pledge our honor that we have abided by the Stevens Honor System. 

//(NOTE!) Include all required libraries here
#include "MQTT.h"
#include <Wire.h>
#include <SSD1306Wire.h>
#include <Ultrasonic.h>
#include <Servo.h> 
#include <math.h>


//(NOTE!) You may create any additional objects needed for OLED, Sensors, or Motors here

//motor pins --> don't change
#define motor_left_pin D3
#define motor_right_pin D6

// object creation for motors
Servo motor_left;
Servo motor_right;

//object creation for ultrasonic sensors
Ultrasonic ultrasonic_middle(D5, D8);
Ultrasonic ultrasonic_right(D4, D7);
Ultrasonic ultrasonic_left(TX, D0);

//object creation for OLED display
SSD1306Wire display(0x3C, D2, D1);

//target coordinates w/ 4 midpoints placed in 
const int x_targets[] = {689, 1566, 2000, 2133, 545, 155};//Sample x-coordinates for targets
const int y_targets[] = {677, 139, 100, 623, 410, 138};//Sample y-coordinates for targets

// target reached counter
int target_index = 0;

//_________________________________________________________________________

//MQTT Setting --> DO NOT TOUCH
const char* mqtt_server   = "192.168.0.2";     
const char* mqtt_username = "user";      
const char* mqtt_password = "Stevens1870";
const int mqtt_port = 1883;

//MQTT Topic 
const String topic     = "EAS011_North";       //North Arena
//const String topic   = "EAS011_South";       //South Arena 
//const String topic = "SmartArena";    //Integration Lab

//WiFi Setting 
const char* ssid       = "TP-Link_8FFD";       //North Arena Check the router on the TA desk.
const char* password   = "68287078";           //North Arena Check the router on the TA desk.
//const char* ssid     = "TP-Link_9402";         //South Arena Check the router on the TA desk.
//const char* password = "77556578";             //South Arena Check the router on the TA desk.
//const char* ssid = "TP-Link_7310";    //Integration Lab
//const char* password = "74952359";    //Integration Lab
//__________________________________________________________________________

//target values --> from ArDuco markers
float x_robot, y_robot, z_ang_robot;

//(NOTE!) You may add additional variables here as needed, but carefully consider variable scope (gloval vs. local)

//values from ultrasonic sensors 

int distance_middle;
int distance_right;
int distance_left;

//setup --> runs every robot start

void setup() {
    Wire.begin();
    delay(25);
    wifi_mqtt_init();
    mqtt_clean();
    delay(500);
    display.init();
    display.flipScreenVertically();
    motor_left.attach(motor_left_pin);
    motor_right.attach(motor_right_pin);
    motor_left.write(90);
    motor_right.write(90);
    delay(2000);

}

// move robot forward
void forward(){
    motor_left.write(110);
    motor_right.write(109);
}

// stop robot --> cut both motors
void stop(){
    motor_left.write(90);
    motor_right.write(90);
}

// adjust robot orientation to the left
void left_correction(){
    motor_left.write(107);
    motor_right.write(120);
}

//adjust robot orientation to the right
void right_correction(){
    motor_left.write(120);
    motor_right.write(107);
}

// turn robot 90 degrees to the left
void turn_left(){
    motor_left.write(70);
    motor_right.write(110);
    delay(720);
}

// turn robot 90 degrees to the right
void turn_right(){
    motor_left.write(110);
    motor_right.write(70);
    delay(720);
}

// reverse the robot
void reverse(){
  motor_left.write(60);
  motor_right.write(60);
}

//used to filter out misreads from the robot --> recommended by office hours TA
unsigned long lastAction = 0;       // Timer for obstacle maneuvers
const unsigned long actionDelay = 300; // Duration for reverse/turn in ms

//loop --> runs continously when robot is powered on
void loop() {
    //MQTT network code --> provided by baseline code
    unsigned long now = millis();
    mqtt_rebound();
    client.loop();

    // 3 ultraosnic sensor readings 
    distance_middle = ultrasonic_middle.read(CM);
    delay(10);
    distance_right  = ultrasonic_right.read(CM);
    delay(10);
    distance_left   = ultrasonic_left.read(CM);
    delay(10);

    // robot distance calculations
    float dx = x_targets[target_index] - x_robot;   //distance between robot and target x-axis
    float dy = y_targets[target_index] - y_robot;   //distance between robot and target y-axis
    float distance_to_target = sqrt(dx*dx + dy*dy); //resultant vector produced by pythagorean theorem

    //robot angle calculations
    float desired_angle = atan2(dy, dx) * 180 / PI; //angle to point robot at target
    float angle_error = desired_angle - z_ang_robot; //current angle error

    // fit the robot angle between 180 
    if(angle_error > 180) angle_error -= 360;
    if(angle_error < -180) angle_error += 360;

    // when all targets reached --> stop the robot
    if(target_index >= 6){
        stop();
        delay(5000);
    }

    // if not all targets have been reached --> run this loop
    if(target_index < 6){

    // when robot close to target, stop the robot
    if(distance_to_target < 100){
            stop();
            target_index++;
            delay(2000);
            lastAction = now;  
        }

    //Collision Logic
    if(distance_middle < 16){
        if(distance_left < distance_right){
            turn_right();
            delay(10);
        }
        else {
            turn_left();
            delay(10);
    }
}
    if(distance_left < 8){
        right_correction();
    }
    if(distance_right < 8){
        left_correction();
    }

    // Path finding logic
    if(angle_error > 15){
        left_correction();
    }
    else if(angle_error < -15){
        right_correction();
    }
    else{
        // if no correction needed --> go straight
        forward();
    }

}
    //print to OLED display --> sensor data and Angle error
    display.setFont(ArialMT_Plain_10);
    display.clear();
    display.drawString(0, 0, "Ultra_Middle: " + String(distance_middle));
    display.drawString(0, 15, "Ultra_Left: " + String(distance_left));
    display.drawString(0, 30, "Ultra_Right: " + String(distance_right));
    display.drawString(0, 45, "Angle_Error: " + String(angle_error));
    display.display();

// MQTT settings --> provided by the basline code
    mqtt_clean();
    while (millis() - now < 100){delay(5);}
}

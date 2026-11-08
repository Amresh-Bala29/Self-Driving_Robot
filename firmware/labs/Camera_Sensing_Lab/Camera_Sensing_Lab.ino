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
//(NOTE!) Include all required libraries here
#include "MQTT.h"
#include <SSD1306Wire.h>
#include <Wire.h>

//(NOTE!) You may create any additional objects needed for OLED, Sensors, or Motors here
SSD1306Wire display(0x3C, D2, D1);

const int x_targets[] = { 1450, 200, 200, 650 };  //Sample x-coordinates for targets
const int y_targets[] = { 150, 600, 150, 150 };   //Sample y-coordinates for targets
int target_index = 0;

//MQTT Setting (Common configuration used across both South Arena and North Arena systems)
const char* mqtt_server = "192.168.0.2";
const char* mqtt_username = "user";
const char* mqtt_password = "Stevens1870";
const int mqtt_port = 1883;

//MQTT Topic
const String topic = "EAS011_North";  //North Arena
//const String topic   = "EAS011_South";       //South Arena

//WiFi Setting
const char* ssid = "TP-Link_8FFD";  //North Arena Check the router on the TA desk.
const char* password = "68287078";  //North Arena Check the router on the TA desk.
//const char* ssid     = "TP-Link_9402";         //South Arena Check the router on the TA desk.
//const char* password = "77556578";             //South Arena Check the router on the TA desk.

float x_robot, y_robot, z_ang_robot;

//(NOTE!) You may add additional variables here as needed, but carefully consider variable scope (gloval vs. local)

void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(25);
  wifi_mqtt_init();
  mqtt_clean();
  display.init();
  display.flipScreenVertically();
  delay(500);

  //(Note!) You may include any system initialization commands here
}

void loop() {
  unsigned long stamp = millis();
  mqtt_rebound();
  client.loop();

  //(Note!) You may include your main commands here, such as the obstacle avoidance & path-planning algorithm and OLED output
  //        The commands already included in the baseline sketch are essential for updating the robot's position and refreshing
  //        the date every time the loop() functions runs. Therefore, do NOT modify the order or placement of thiese commands.


  mqtt_clean();
  while (millis() - stamp < 100) { delay(5); }  //Sticks into an empty loop until it has been more than 100ms.
  //In asynchronous programming, it is important to free the CPU with delays while it's doing nothing.


    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "X-Cord:");
    display.drawString(0, 18, "Y-Cord:");
    display.drawString(0, 36, "Angle:");

    display.drawString(90, 0, String(x_robot));
    display.drawString(90, 18, String(y_robot));
    display.drawString(90, 36, String(z_ang_robot));

    display.display();
    display.clear();

  delay(120);
}

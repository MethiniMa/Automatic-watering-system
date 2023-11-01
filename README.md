# Automatic-Watering-System

## Introduction
In this era of rapid technological development, human beings pursue a convenient and simple life, and much-automated equipment has also been developed in agriculture to save manpower and time. One of them is the automatic watering system. When the scale of the farm is very large, the farmers are able to use this device to regularly observe the state of crops and give the appropriate amount of fertilizer and water. Therefore, we are going to develop the simplified model of the automatic watering system by applying both hardware and software which are Arduino, STM32F4and Blynk applications.

## Objective
The purpose of this project is to combine the hardware and software to complete the watering system which can be controlled by the application. The Arduino receives the data from the Blynk application via ESP32 and displays on the LCD screen. Also, the Arduino sends the data which obtain from the sensor to Blynk application. The functions of this system are listed below.
1. Set up the day and time for the routine checking
2. Inspect the moisture of plants according to the setting. 
3. Switch on/ off the solenoid valve to provide two types of fertilizer.

# Apparatus & Application
-Blynk(Software)
1. Select two modes of the system: automatic/manual
2. Set the period of watering time
3. View the status of moisture and fertilizers
4. Control the solenoid switch 

-STM32F4 (Hardware)
  -> Display the status from the Blynk application
1. Show the current date, time, and the day
2. Show the moisture and two pumps are working or not
3. Select the manual mode for user can turn on/off the valve and the pumps.
    (If user does not select the “manual mode”, meaning that the system is in the automatic mode.

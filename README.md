**Project Title**: Drone Weather Station			

**Project Description**: 

This research project investigates the utilization of drone technology equipped with various sensors for the detection and real-time monitoring of atmospheric conditions, specifically measuring **temperature, humidity, barometric pressure, light, UV radiation, infrared levels,** and **gas concentrations**. The drones are designed to gather data from remote locations, specifically within the boundary layer—the lowest layer of the Earth's atmosphere—by measuring critical variables such as temperature, humidity, and wind speed. The sensing apparatus includes a temperature and humidity sensor, as well as a UV sensor that assesses the intensity of ultraviolet radiation. Collected data is transmitted to a microcontroller, which is programmed to receive operational commands from a laptop or mobile device via a Bluetooth module. By employing an Arduino microcontroller, the system enables automated data collection and real-time data transmission, allowing the drone to autonomously execute flight instructions. This project aims to provide remote communities with the capability to independently monitor local weather patterns, enhancing their resilience and capacity for informed decision-making regarding agriculture and disaster preparedness.

### **Components Required**

* Quadcopter frame (4-axis)  
* 2312 770KV BLDC (Brushless DC Motors)  
* BLHeli 14.7 25A ESC (Electronic Speed Control)  
* APM 2.8 Flight Controller (Ardupilot Mega)  
* GPS module \+ Compass  
* 11.1V 3000 mAh LiPo battery  
* 30A ESC Speed Controller (BEC 2A for Multirotor Quadcopter, 4 Pack)  
* RC Controller (6-channel)  
* Props (any 1045\)  
* 32/64 GB micro SD card (any brand)  
* 6-channel SBUS Radio Set  
* B3 Pro charger  
* Connecting wires  
* Glue gun  
* Glue sticks

**Sensors:**

* Temperature sensors  
* Barometric pressure sensors  
* Humidity sensors  
* Dew point sensors  
* Light sensors  
* UV sensors  
* Infrared sensors  
* Wind speed and direction sensors

**Libraries used with Adrupilot**:

1. [AP\_Motors](https://github.com/ArduPilot/ardupilot/tree/master/libraries/AP_Motors): Multicopter and traditional helicopter motor mixing.  
2. [RC\_Channel](https://github.com/ArduPilot/ardupilot/tree/master/libraries/RC_Channel): A library to convert PWM input/output from APM\_RC into internal units such as angles.  
3. [AC\_PID](https://github.com/ArduPilot/ardupilot/tree/master/libraries/AC_PID) : PID(Proportional-Integral-Derivative) controller library  
4. [AP\_Math](https://github.com/ArduPilot/ardupilot/tree/master/libraries/AP_Math): Various math functions are handy for vector manipulation.  
5. [AP\_Common](https://github.com/ArduPilot/ardupilot/tree/master/libraries/AP_Common): Core includes required by all sketches and libraries.  
     
   

 **Software Used:** 

1. To configure the APM we need to have the Mission planner software.  
2. 3DR Tower App to use functions of the quadcopter.  
3. C Programming on Ubuntu.

**Note**: This video captures a drone flying inside a room within a house, demonstrating key features such as temperature and humidity readings, which take approximately 250 milliseconds to measure. The footage also includes structural inspections of the room's interior and tests for the drone's flight stability in a confined space.

The only person who can really motivate you is you. Happy learning\!

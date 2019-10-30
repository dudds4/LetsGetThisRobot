# MTE 380 - Mechatronics Engineering Design

This repository contains all the code used for our MTE 380 design project. MTE 380 is a project course designed to prepare 3rd year engineering students at the University of Waterloo for their fourth year capstone design projects. 

Typically in the course, students design a robot to navigate around some kind of course. Students form groups of four, and design every aspect of the robot themselves, with only a few limitations; that the robot be controlled by an arduino or microprocessor of similar compute power, and that the project budget stay below some threshold.


<p align="center">
  <img width="460" height="300" src="https://raw.githubusercontent.com/dudds4/LetsGetThisRobot/master/old_robot_design.png">
</p>
<p align="center">
 <i>  intermediate robot design </i>
</p>

Confusingly, the actual code used on test day lives in /Testing/initialtesting. The most important portions of the code were:
 - sensors.h/cpp: initializing and measuring from various sensors.
 - util.h/cpp implements various closed-loop control method, using sensors as feedback. Useful functions include 'turnOnSpot', 'driveStraight', and 'wallFollow' which use IMU and IR readings to determine how to set motor voltages.
 - basefinding, rampfinding, rampclimbing, and homefinding .h/cpp files implement finite state machines for different portions of the course which were to be navigated.

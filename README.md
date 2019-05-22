# erwhi_remars_2019
This package contains software used at AWS Re:MARS 2019
<br>
## Simulation
Use this launch file:<br>
`roslaunch erwhi_remars_2019 remars_gazebo.launch`<br>
Robot explore environment for 120 seconds.<br>
After that robot will jump at initial position and everything is resetted.<br>
<br>
<br>

## Robot
Just launch:<br>
`roslauch erwhi_remars_2019 robot_demo.launch`<br>
Robot explore autonomously, you can see a blinking yellow led during exploration.<br>
During navigation if something is detected blue led turn on.<br>
If an internet connection is avalable following metrics will be shared on AWS Cloudwatch:
- battery voltage;
- RAM usage;
- number of objects detected.

When you disable exploration yellow blinky led turn off and a red led turn on.  After 300 seconds (5 minutes) red light turn off, yellow one blinks and robot is autonomous again.<br>
If you look that SLAM is not working you can reset RTABmap.<br>
Check [gamepad controls](https://github.com/gbr1/erwhi_remars_2019/wiki/Gamepad-controls) to learn how interact with Erwhi in demo mode.<br>
<br>
<br>
<br>

***Copyrights (c) 2019 Giovanni di Dio Bruno under MIT license.***

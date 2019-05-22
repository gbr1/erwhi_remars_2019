/*
 * The MIT License
 *
 * Copyright (c) 2019 Giovanni di Dio Bruno https://gbr1.github.io
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <ros/ros.h>
#include <upboard_ros/Leds.h>
#include <upboard_ros/Led.h>

bool status = true;
float t = 0.1;

int main(int argc, char **argv){
    ros::init(argc, argv, "explore_blinker");
    ros::NodeHandle nh;
    ros::Publisher leds_pub = nh.advertise<upboard_ros::Leds>("/upboard/leds", 10);

    upboard_ros::Leds msg;
    upboard_ros::Led ledmsg;

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    while(ros::ok()){

        current_time = ros::Time::now();
        double dt = (current_time - last_time).toSec();
        if (dt>=t){
            msg.header.stamp=ros::Time::now();
            msg.header.frame_id="base_link";
            ledmsg.led=ledmsg.YELLOW;
            ledmsg.value=status;
            msg.leds.push_back(ledmsg);
            ledmsg.led=ledmsg.RED;
            ledmsg.value=false;
            msg.leds.push_back(ledmsg);
            leds_pub.publish(msg);
            msg.leds.clear();
            
            t=0.1+0.8*status;
            status=!status;

            last_time = ros::Time::now();
        }
        ros::spinOnce();
    }

    return 0;
}
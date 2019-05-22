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
#include <ros_openvino/Objects.h>

upboard_ros::Leds ledsmsg;
upboard_ros::Led ledmsg;

bool publish=false;
bool clear=false;


void callbackDetection(const ros_openvino::Objects & msg){
    if (!msg.objects.empty()){
        ledsmsg.header.stamp=ros::Time::now();
        ledsmsg.header.frame_id="base_link";
        ledmsg.led=ledmsg.BLUE;
        ledmsg.value=true;
        ledsmsg.leds.push_back(ledmsg);
    }
    else{
        ledsmsg.header.stamp=ros::Time::now();
        ledsmsg.header.frame_id="base_link";
        ledmsg.led=ledmsg.BLUE;
        ledmsg.value=false;
        ledsmsg.leds.push_back(ledmsg);
    }
    publish=true;

}

int main(int argc, char **argv){
    ros::init(argc, argv, "led_detection");
    ros::NodeHandle nh;
    ros::Publisher leds_pub = nh.advertise<upboard_ros::Leds>("/upboard/leds", 10);
    ros::Subscriber sub = nh.subscribe("/object_detection/results",10,callbackDetection);

    /*
    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();
    */

    while(ros::ok()){
        /*
        if (clear){
            current_time = ros::Time::now();
            double dt = (current_time - last_time).toSec();
            if (dt>=0.1){
                ledsmsg.header.stamp=ros::Time::now();
                ledsmsg.header.frame_id="base_link";
                ledmsg.led=ledmsg.BLUE;
                ledmsg.value=false;
                ledsmsg.leds.push_back(ledmsg);
                leds_pub.publish(ledsmsg);
                ledsmsg.leds.clear();
                clear=false;
            }
            
        }
        */
        if (publish){
            leds_pub.publish(ledsmsg);
            ledsmsg.leds.clear();
            publish=false;
            /*
            clear=true;
            last_time=ros::Time::now();
            */
        }
        
        
        ros::spinOnce();
    }

    return 0;
}
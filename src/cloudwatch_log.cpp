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
#include <ros_monitoring_msgs/MetricList.h>
#include <ros_monitoring_msgs/MetricData.h>
#include <ros_monitoring_msgs/MetricDimension.h>
#include <sengi_ros/Status.h>
#include <ros_openvino/Objects.h>

#include <sys/sysinfo.h>



struct sysinfo sys_info;

float voltage=0.0;
int object_detected = 0;


void callbackSengi(const sengi_ros::Status & msg){
    voltage=msg.battery_voltage;
}

void callbackDetection(const ros_openvino::Objects & msg){
    object_detected+=msg.objects.size();
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "cloudwatch_metrics_producer");
    
    ros::NodeHandle nh;

    ros::Subscriber sub=nh.subscribe("/sengi/status", 10, callbackSengi);
    ros::Subscriber sub_obj=nh.subscribe("/object_detection/results", 10, callbackDetection);

    ros::Publisher pub=nh.advertise<ros_monitoring_msgs::MetricList>("/metrics",1);

    ros_monitoring_msgs::MetricData data_msg;
    ros_monitoring_msgs::MetricList list_msg;

    ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time = ros::Time::now();

    while(ros::ok()){
        current_time = ros::Time::now();
        double dt = (current_time - last_time).toSec();
        if (dt>=10){
            data_msg.metric_name="RAM usage";
            sysinfo(&sys_info);
            data_msg.value=((float)((uint64_t)sys_info.totalram-(uint64_t)sys_info.freeram))/1024/1024/1024.0;
            data_msg.unit=data_msg.UNIT_GIGABYTES;
            data_msg.header.frame_id="base_link";
            data_msg.header.stamp=ros::Time::now();
            data_msg.time_stamp=ros::Time::now();

            list_msg.metrics.push_back(data_msg);


            data_msg.metric_name="BATTERY status";
            data_msg.value=voltage;
            data_msg.unit=data_msg.UNIT_NONE;
            data_msg.header.frame_id="base_link";
            data_msg.header.stamp=ros::Time::now();
            data_msg.time_stamp=ros::Time::now();

            list_msg.metrics.push_back(data_msg);

            data_msg.metric_name="Detection";
            data_msg.value=object_detected;
            data_msg.unit=data_msg.UNIT_NONE;
            data_msg.header.frame_id="base_link";
            data_msg.header.stamp=ros::Time::now();
            data_msg.time_stamp=ros::Time::now();

            list_msg.metrics.push_back(data_msg);

            pub.publish(list_msg);
            list_msg.metrics.clear();
            object_detected=0;

            last_time = ros::Time::now();

        }
        ros::spinOnce();
    }
}

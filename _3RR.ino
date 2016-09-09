/*
 *  Filename: _3IRR.ino
 *    Author: Mohamed HNEZLI
 *     Brief: Handling N IRreceivers (here N=3) using muxer(s) to guide the robot
 *            straight to either docking-station and publishing to rostopic.
 *   Version: September 2016
 *      Note: Please refer to the schematic to check needed connections
 *            and make sure of reducing side sensors interference
 */


/*******************************
 *     Needed header files     *
 *******************************/
#include <IRremote.h>

#include <ros.h>
#include <ros/time.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Range.h>


 /*******************************
 *    Variables declaration    *
 *******************************/
// Need to declare only one IRrecv object (library issue)
#define RECV_PIN 2
IRrecv sensor(RECV_PIN);
decode_results sensor_rslt;

// Muxer pins (here I'm using an 8to1 one) and three sensors
const int select1 = 11, select2 = 12;

// direction values: 1..7 \6
int dir = 0;
float prev = 0.4;
bool stopper_var = false;


/*******************************
 *         Ros utilities       *
 *******************************/
 // ros handler
ros::NodeHandle  nh;

// driver publisher
geometry_msgs::Twist driver_msg;
ros::Publisher driver_pub("/cmd_vel", &driver_msg);

// Callback funtion
void stopper_cbf ( const sensor_msgs::Range& IR_msg )
{
  // Checking for obstacles
  if (IR_msg.range <= 10 )
      stopper_var = true;

  return;
}

// stopper subscriber
ros::Subscriber<sensor_msgs::Range> stopper_sub("/IR3", &stopper_cbf);


void setup()
{
  // Serial.begin(9600);

  // Start the receiver
  sensor.enableIRIn();
 
  // muxer pins
  pinMode(select1, OUTPUT);
  pinMode(select2, OUTPUT);
 
  // ros onbjects initialization
  nh.initNode();
 
  nh.advertise(driver_pub);
  driver_msg.linear.x=0;
  driver_msg.linear.y=0;
  driver_msg.linear.z=0;
  driver_msg.angular.x=0;
  driver_msg.angular.y=0;
  driver_msg.angular.z=0;

  nh.subscribe(stopper_sub);
}

/*
 * Please adjust delays to the robot's velocity to avoid crashs
 */
void loop()
{
  // reading from middle sensor
  digitalWrite(select1, HIGH);
  if (sensor.decode(&sensor_rslt)) {
    sensor.resume(); // Receive the next value
    if(sensor_rslt.value != 0)  dir = 1;
  }
  delay(90);

  // reading from right sensor
  digitalWrite(select2, HIGH);
  if (sensor.decode(&sensor_rslt)) {
    sensor.resume(); // Receive the next value
    if(sensor_rslt.value != 0)  dir += 2;
  }
  delay(90);
 
  // reading from left sensor
  digitalWrite(select1, LOW);
  if (sensor.decode(&sensor_rslt)) {
    sensor.resume(); // Receive the next value
    if(sensor_rslt.value != 0)  dir += 4;
  }
  delay(90);
 
  digitalWrite(select2, LOW);
 
  /*
   * dir:  1: || uls          7: || ls         
   *       2: <-              3: \\
   *       4: ->              5: //
   */
  switch (dir)
  {
    case 1:  //Serial.println(" || uls ");
             driver_msg.linear.x = 0.09;
             driver_msg.angular.z = 0.0;
             for (short i=0; i<50; i++)
             {
                driver_pub.publish(&driver_msg);
                nh.spinOnce();
             }
             break;
    case 2:  //Serial.println(" <- ");
             driver_msg.linear.x = 0.0;
             driver_msg.angular.z = (-0.3);
             for (short i=0; i<100; i++)
             {
                driver_pub.publish(&driver_msg);
                nh.spinOnce();
             }
             prev = (-0.4);
             break;
    case 3:  //Serial.println(" \\\\ ");
             driver_msg.linear.x = 0.15;
             driver_msg.angular.z = (-0.3);
             for (short i=0; i<50; i++)
             {
                driver_pub.publish(&driver_msg);
                nh.spinOnce();
             }
             prev = (-0.4);
             break;
    case 4:  Serial.println(" -> ");
             driver_msg.linear.x = 0.0;
             driver_msg.angular.z = 0.3;
             for (short i=0; i<100; i++)
             {
                driver_pub.publish(&driver_msg);
                nh.spinOnce();
             }   
             prev = 0.4;
             break;
    case 5:  Serial.println(" // ");
             driver_msg.linear.x = 0.15;
             driver_msg.angular.z = 0.3;
             for (short i=0; i<50; i++)
             {
                driver_pub.publish(&driver_msg);
                nh.spinOnce();
             }   
             prev  = 0.4;
             break;
    case 7:  Serial.println(" || ls ");
             driver_msg.linear.x = 0.1;
             driver_msg.angular.z = 0.0;
             for (short i=0; i<25; i++)
             {
                driver_pub.publish(&driver_msg);
                nh.spinOnce();
             }   
             break;
    default: //Serial.println(" nothing ");
             driver_msg.linear.x = 0.0;
             driver_msg.angular.z = prev;
             driver_pub.publish(&driver_msg);
             nh.spinOnce();
             break;
  }

  if (stopper_var == true )
  {
    /*for (short i=0; i<100; i++)
    {
             driver_msg.linear.x = (-0.2);
             driver_msg.angular.z = 0.0;
             driver_pub.publish(&driver_msg);
             nh.spinOnce();
    }*/
             while (1){    driver_msg.linear.x = 0.0;
             driver_msg.angular.z = 0.0;
             driver_pub.publish(&driver_msg);
             nh.spinOnce();}
    return;
  }
  dir = 0;
 // delay(500);
}

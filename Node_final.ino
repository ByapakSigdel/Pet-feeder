#include <Simpletimer.h>
#include<Ticker.h>

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLLEZTns6T"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "PmkqHjmvTEhSfngESMC_9vmnc3_X7Ka7"



//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WidgetRTC.h>
#include <TimeLib.h>
Ticker timer;


SimpleTimer timer1;
WidgetRTC rtc;
//Brett setup stepper
#include <Stepper.h>
// change this to the number of steps on your motor
#define STEPS 400
// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 5, 4, 0, 2);
int start = 14;
int hopperLevel = 34; // Hopper hold 34 feeds
int val = 200; // steps feed wheel 1/6th of a turn


int timerTrigger = 0;
int appTrigger = 0;
int timerEnable = 0;
int isFirstConnect = true;

// Brett end setup stepper

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "PmkqHjmvTEhSfngESMC_9vmnc3_X7Ka7";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "TP-LINK_2616";
char pass[] = "83335112";



// Digital clock display of the time and date feeder started


void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
 // Begin synchronizing time
  rtc.begin();

  //Brett
   
  stepper.setSpeed(100); // set the speed of the motor
  pinMode(14, INPUT); // start feeding
  // turns off stepper motor at start
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(0,LOW);
  digitalWrite(2,LOW);
  Blynk.virtualWrite(V1, 34); // sets hopper level to 34 on startup
  
  Blynk.virtualWrite(V9, timerTrigger);// sets timerTrigger count to 0 on start
  Blynk.virtualWrite(V10, appTrigger);// sets appTrigger count to 0 on start
 


  
 
}

BLYNK_WRITE(V0)
{
  if(param.asInt() == 1) {
    doFeed();
  }
}

 BLYNK_WRITE(V8)// timer enable button
{
  if(param.asInt() == 1) {
    timerEnable = 1;
//     Serial.print("Timer enable: ");
//  Serial.println(param.asInt());
  }
    else
    {
    timerEnable = 0;
    
   // Serial.print("Timer enable: ");
 // Serial.println(param.asInt());
    
  }
} 



BLYNK_WRITE(V6) //1st Timer Feed
{
  // You'll get HIGH/1 at startTime and LOW/0 at stopTime.
  // this method will be triggered every day
  // until you remove widget or stop project or
  // clean stop/start fields of widget
  //Serial.print("Got a value: ");
 // Serial.println(param.asInt());

  
  Serial.print(param.asStr());
  Serial.println();
  int a = param.asInt();
  int t = (hour()*60+minute())*60+second();
  Serial.print(t);

  int diff = a-t;

 
  timer.attach(diff,timerCallBack);
 
  // only operate if Timer Ctrl Button is on
  
  
}


void timerCallBack()
{
doTimerFeed();
timer.detach();

}
BLYNK_WRITE(V7) //2nd Timer Feed
{
  // You'll get HIGH/1 at startTime and LOW/0 at stopTime.
  // this method will be triggered every day
  // until you remove widget or stop project or
  // clean stop/start fields of widget
  //Serial.print("Got a value: ");
 // Serial.println(param.asInt());
 

  
  Serial.print(param.asStr());
  Serial.println();
  int a = param.asInt();
  int t = (hour()*60+minute())*60+second();
  Serial.print(t);

  int diff = a-t;


  timer.attach(diff,timerCallBack);
 
}
 

 void doFeed() // rotate feeder 1/6th revolution to release feed
 {
 // turns feed flaps 1/6th turn
  stepper.step(val);
 // turns off stepper motor
   digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(0,LOW);
  digitalWrite(2,LOW);
  //hopperLevel = hopperLevel - 1 ;
   //Blynk.virtualWrite(V1, hopperLevel);
   appTrigger = appTrigger + 1 ;// add 1 to appTrigger count
   Blynk.virtualWrite(V10, appTrigger);
   lastFedclock();
 
 }



void doTimerFeed() // Timer feed rotate feeder 1/6th revolution to release feed
// updates hopper level and timertrigger counter
 {
 // turns feed flaps 1/6th turn
  stepper.step(val);
 // turns off stepper motor
   digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(0,LOW);
  digitalWrite(2,LOW);
  hopperLevel = hopperLevel - 1 ;
   Blynk.virtualWrite(V1, hopperLevel);

  timerTrigger = timerTrigger + 1 ;
  Blynk.virtualWrite(V9, timerTrigger);
  lastFedclock();
   
 }

 








 // Digital clock display of the time and date last fed (man app or timer)
void lastFedclock()
{

 //Brett store stert time and date last fed
 // Time
 String fedhour = String(hour(), DEC);
  int hourdigits = fedhour.length();
  if(hourdigits == 1)
  {
    fedhour = "0" + fedhour;
  }
 String fedminute = String(minute(), DEC);
  int minutedigits = fedminute.length();  
  if(minutedigits == 1)
  {
    fedminute = "0" + fedminute;
  }

//Date

 String fedday = String(day(), DEC);
  int daydigits = fedday.length();
  if(daydigits == 1)
  {
    fedday = "0" + fedday;
  }
 String fedmonth = String(month(), DEC);
  int monthdigits = fedmonth.length();  
  if(monthdigits == 1)
  {
    fedmonth = "0" + fedmonth;
  }


 
  String fedTime = fedhour + ":" + fedminute;
  String fedDate = fedday + "/" + fedmonth + "/" + year();

  // Send time to the App
  Blynk.virtualWrite(V11, fedTime);
  // Send date to the App
  Blynk.virtualWrite(V12, fedDate);
  
}

  // End Digital clock display of the time and date last fed (man app or timer)
 

 




void loop()
{
  Blynk.run();
  timer1.run();

    // get the man switch value
  if (digitalRead(start) == HIGH) // feeds and updates the hopper level/man trigger count on manual trigger
 {
 

   
 }

}

/********Arduino controlled Three Servo Hexapod Robot with a personality!***
*************Coded and tested successfully by Sidharth Makhija***********
**************https://droidhangar.wordpress.com/contact/*****************
********************Released AS-IS under MIT Licence***************************
*************************************************************************
***********************************************************************/


#include <Servo.h>

Servo lift;
Servo left;
Servo right;
//Sensor and LED Pins
int led = 5;          
int brightness = 0;    
int fadeAmount = 5;    
int ledH=4;
int trig = 8;
int echo = 7;
int ldrpin=A0;

//Variables
int distFront;
unsigned long pulsetime =0;
int ldr_read=0;
int amb_ldr=0;//ambient reading
int diff_ldr=0;//difference in ldr reading
double bat_read=0;//battery voltage reading
int dist_read=0;//distance reading

void setup()
{
  //Attach Servos
  lift.attach(10);
  left.attach(9);
  right.attach(11);
  
  //Servo Center positions
  startPos();
  
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(led, OUTPUT);
  pinMode(ledH,OUTPUT);
  
  //Starting blinking of headlights
  beep(ledH);
  analogWrite(led,75);
  amb_ldr=ldrReading();//Take ambient ldr reading
  delay(2000);
}

void loop()
{
  analogWrite(led,75);
 ldr_read=ldrReading();
 diff_ldr=amb_ldr-ldr_read;//Calculate difference in light intesity
 dist_read=readDistance();//Take distance reading
 bat_read=batteryCheck();//Take battery voltage reading
 
 if(bat_read>7.2)//If voltage greater than 7.2V
 {
   if(diff_ldr<200)//Check if being petted 
    {
      if(dist_read>10) {forward();}
      else {
      turn();
     //delay(1000);
           }
    }
    else {petMode();}
 }
 else {autoShut();}//Not enough battery
}

//////////////////////////////////////////////////////
void forward()
{
   firstHalf();
  delay(200);
  secondHalf();
  delay(200);
}
void turn()
{
  firstTurn();
  delay(200);
  secondTurn();
  delay(200);
}

int ldrReading()
{
  int val = analogRead(ldrpin);
  return val;
}

void petMode()
{
  startPos();
  beep(led);
  happyShake();
}

void firstHalf()
{
  lift.write(95);//right up
  delay(100);
  left.write(80);//left bk
  right.write(30);//right fwd
}

void secondHalf()
{
  lift.write(145);//left up
  delay(100);
  left.write(150);//left fwd
  right.write(90);//right bk
}

void beep(int pin)
{
  for(int i =0;i<3;i++)
  {
    digitalWrite(pin,HIGH);
    delay(200);
    digitalWrite(pin,LOW);
    delay(200);
  }
}


double batteryCheck()
{
  const float referenceVolts = 5;        // the default reference on a 5-volt board
//const float referenceVolts = 3.3;  // use this for a 3.3-volt board

const float R1 = 1000;//955 // value for a maximum voltage of 10 volts
const float R2 = 1000;//955
// determine by voltage divider resistors, see text
const float resistorFactor = 1023.0 / (R2/(R1 + R2));  
const int batteryPin = A2;         // +V from battery is connected to analog pin 0
int val = analogRead(batteryPin);  // read the value from the sensor
   double volts = (val / resistorFactor) * referenceVolts*4; // calculate the ratio
   return volts;
}

int readDistance()
{
  digitalWrite(trig, LOW); 
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);       
  delayMicroseconds(10);               
  digitalWrite(trig,LOW);         
  pulsetime = pulseIn(echo, HIGH);  
  return pulsetime/58.2;                
}

void fade()
{
  analogWrite(led, brightness);    
  brightness = brightness + fadeAmount; 
  if (brightness == 0 || brightness == 255) 
  {
    fadeAmount = -fadeAmount ; 
  }     
 delay(30);
}

void autoShut()
{
  lift.detach();
  left.detach();
  right.detach();
  fade();  
}

void firstTurn()
{
  lift.write(95);//right up
  delay(100);
  left.write(150);//fwd
  right.write(30);//fwd
}

void secondTurn()
{
  lift.write(145);//left up
  delay(100);
  left.write(80);//bk
  right.write(90);//bk
}

void startPos()
{
  lift.write(120);
  left.write(120);
  right.write(56);
}

void happyShake()
{
  digitalWrite(13,HIGH);
   lift.write(60);//right up
  delay(200);
  digitalWrite(13,LOW);
  lift.write(180);//left up
  delay(200);
}
  

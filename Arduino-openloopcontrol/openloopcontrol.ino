#define LED 13

#define dc_enable1 2
#define dc_enable2 3
#define pulse_pin 23
#define dir_pin 22
#define enable_pin 11

int recva,recvb;
int recv;
char checkchar;

int microstep, pwm, flag = 0;
int prevStep = 0;
int currentStep = 0;
//int prevSpeed = 0;
int stepLimit = 15;
//int speedLimit = 200;
bool prevDir = LOW;
//bool dir;

bool getDirectionLR(int recvb)
{
  if (recvb >= 128)
    return HIGH;
  else
    return LOW;    
}

int getDirectionMag(int recvb)
{
  return (recvb & 127);
}

bool getDirectionSpeed(int recva)
{
  if (recva >= 128)
    return HIGH;
  else
    return LOW;  
}

int getSpeed(int recva)
{
  return (recva & 127) * 2;
}

void dc_write(int properSpeed, bool directionSpeed)
{
  analogWrite(dc_enable1, properSpeed);
  analogWrite(dc_enable2, properSpeed);
  if (!directionSpeed)
  {
  digitalWrite(24, LOW);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(27, LOW);
  }
  else
  {
  digitalWrite(24, HIGH);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, HIGH);   
  }
}

void stepper_write(bool dir, int microstep)
{
  int i = 0;
  if(dir == HIGH)
    digitalWrite(dir_pin, HIGH);
  else if(dir == LOW)
    digitalWrite(dir_pin, LOW);
  for(i=0; i<microstep; i++)
  {
    digitalWrite(pulse_pin, HIGH);
    delayMicroseconds(500);
    digitalWrite(pulse_pin, LOW);
    delayMicroseconds(40000);
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(dc_enable1, OUTPUT);
  pinMode(dc_enable2, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(pulse_pin,OUTPUT);
  pinMode(dir_pin,OUTPUT);
  pinMode(enable_pin,OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
    if (Serial.available() >= 3)
  {
    checkchar = Serial.read();

    if (checkchar == 'w')
    {
      recva = 0;  //speed
      recvb = 0;  //direction
      recva = Serial.read();
      recvb = Serial.read();

      bool directionSpeed = getDirectionSpeed(recva);
      int properSpeed = getSpeed(recva);
      bool directionLR = getDirectionLR(recvb);
      int directionMag = getDirectionMag(recvb);

      dc_write(properSpeed, directionSpeed);
      
//      if (revB >= 128)
//      {
//        dir = 1;
//        revB = revB - 128;
//      }
//      microstep = revB;
      microstep = directionMag;
      if (microstep > stepLimit)
      {
        currentStep = stepLimit;
        microstep = 0;
        return;
      }
      
      if ((prevDir == directionLR) && (prevStep <= microstep)){
        currentStep = microstep;
        microstep = microstep - prevStep;
      }
      else if ((microstep == 0) && (prevDir == HIGH)){
        currentStep = microstep;
        microstep = prevStep;
        directionLR = LOW;
      }
      else if ((microstep == 0) && (prevDir == LOW)){
        currentStep = microstep;
        microstep = prevStep;
        directionLR = HIGH;
      }
      else if ((prevDir == directionLR) && (prevStep > microstep)){
        directionLR = !directionLR;
        currentStep = microstep;
        microstep = prevStep - microstep;
        flag = 1;
      }
      else if ((prevDir != directionLR) && (prevStep <= microstep)){
        currentStep = microstep;
        microstep = prevStep + microstep;
      }
      
      prevStep = currentStep;

      stepper_write(directionLR, microstep);
      
      if(flag == 1)
      {
        directionLR = !directionLR;
        flag = 0;
      }
      
      prevDir = directionLR;
    }
  }

}

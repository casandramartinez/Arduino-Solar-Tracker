#include <AFMotor.h>

const int light_pin = A2;
const int pot = A0;
float max_light = 0;
bool max_found = false;
float max_step;
float step = 0;
int loopN = 0;
int pot_value;
AF_DCMotor dc_motor(3);
AF_Stepper step_motor(48, 1);

void setup() 
{
  Serial.begin(9600);
  Serial.println("start");
  pinMode(light_pin, INPUT);
  dc_motor.setSpeed(200);
  step_motor.setSpeed(20);
  //Calibration of the DC motor to the starting position

  calibrate(pot);

}


void loop() 
{
  //Solar panel is calibrated in starting position
  // Read the ADC, and calculate voltage and resistance from it
  loopN = loopN+1;
  //Serial.println("loop: "+ String(loopN));
  int light_intensity = analogRead(light_pin);
  pot_value = analogRead(pot);
  uint8_t i;
  if (light_intensity > 0){
    float lightV = light_intensity;
    //Serial.println("*******");
    //Serial.println("Light Value: " + String(lightV));

    //Stepper moves to midway until turning back
    step = step+10;
    step_motor.step(10,FORWARD,SINGLE);
    //Serial.println("step: " + String(step));

    if(step>=258){
      //Motor turns at midpoint
      step_motor.step(258,BACKWARD,SINGLE);
      step = 0;
      delay(1000);
    }
    
    checkMax(lightV);

    delay(500);
  }

  int dc_angle = map(pot_value, 1023, 924, 0, 180);
  int target_angle = max_step * (360.0 / 518);
  Serial.println("solar cell angle: " + String(dc_angle));
  Serial.println("max light angle: " + String(target_angle));
  //Serial.println("pot: " + String(pot_value));
  delay(10);

  if((abs(dc_angle - target_angle) > 10) && (loopN >= 26)){
    if((dc_angle - target_angle) < 0){
      //Serial.println("forward");
      dc_motor.run(FORWARD);
      for (i=0; i<255; i++) {
        dc_motor.setSpeed(i);  
        delay(10);
      }
      for (i=255; i!=0; i--) {
        dc_motor.setSpeed(i);  
        delay(10);
      }
    }
    else if((dc_angle - target_angle) > 0){
      //Serial.println("backward");
      dc_motor.run(BACKWARD);
      for (i=0; i<255; i++) {
        dc_motor.setSpeed(i);  
        delay(10);
       }
    
      for (i=255; i!=0; i--) {
        dc_motor.setSpeed(i);  
        delay(10);
      }
    }
  }
  
  else{
      //Serial.println("stop");
      for (i=200; i!=0; i--) {
        dc_motor.setSpeed(i);  
        delay(10);
      }
      dc_motor.run(RELEASE);
      delay(10);
    }
  }

void checkMax(int lightV) {
  //Max Function for light
    if ((lightV - max_light) > 10){
      max_light = lightV;
      max_step = step;
      
      Serial.println("Light Max: " + String(max_light));
      bool maxFound = true;
    }
}

void calibrate(float pot){
  uint8_t i;
  while(analogRead(pot_value) < 1015){
    dc_motor.run(BACKWARD);
    for (i=0; i<255; i++) {
      dc_motor.setSpeed(i);  
      delay(10);
    }
  }
    for (i=255; i!=0; i--) {
      dc_motor.setSpeed(i);  
      delay(10);
 } 
}














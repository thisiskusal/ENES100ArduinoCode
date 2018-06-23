#include "dfr_tank.h"

DFRTank::DFRTank() {
  _left_motor_pwm = 0;
  _right_motor_pwm = 0;
};

void DFRTank::init(){
  pinMode(DFRTANK_E1, OUTPUT);
  pinMode(DFRTANK_M1, OUTPUT);
  pinMode(DFRTANK_E2, OUTPUT);
  pinMode(DFRTANK_M2, OUTPUT);
};

void DFRTank::setLeftMotorPWM(int pwm) {
  
  analogWrite (DFRTANK_E1, abs(pwm));      //PWM Speed Control
  
  if(pwm >= 0.0f) {
    digitalWrite(DFRTANK_M1,HIGH); 
  } 
  else {
    digitalWrite(DFRTANK_M1,LOW);
  }
  _left_motor_pwm = pwm;
};

void DFRTank::setRightMotorPWM(int pwm) {
  analogWrite (DFRTANK_E2, abs(pwm));      //PWM Speed Control
  
  if(pwm >= 0) {
    digitalWrite(DFRTANK_M2,HIGH); 
  } 
  else {
    digitalWrite(DFRTANK_M2,LOW);
  }
  _right_motor_pwm = pwm;
};

void DFRTank::turnOffMotors(){
  digitalWrite(DFRTANK_E1,LOW);
  digitalWrite(DFRTANK_E2,LOW);
  _right_motor_pwm = 0;
  _left_motor_pwm = 0;
};

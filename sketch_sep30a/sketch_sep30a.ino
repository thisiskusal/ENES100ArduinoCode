#include "Enes100.h"

// Global variables
float theta = 5*PI/6; // initial angle in rad
float theta_desired = 0; // Enter the target angle in the space here
float k = -1; // proportional controller for turning speed (“gain”)
float dt = 0.25; // update time interval in seconds
float err; // variable to hold the error
float w; // angular velocity variable (rad/s)
int i = 0; // iteration count

void setup() {
  Serial.begin(9600);
  err = theta - theta_desired; // compute the initial error
}

void loop() {
  if (err > 0.005 || err < -0.005) // only compute if not converged
  {
    i++; // update iteration number, redefines i = i + 1
    w = k*err; // update angular velocity
    theta += w*dt; // update theta, redefines theta = theta + w*dt
    err = theta - theta_desired; // update error
    Serial.print("Iteration ");
    Serial.print(i);
    Serial.print(": Theta = ");
    Serial.print(theta);
    Serial.print(", error = ");
    Serial.println(err);
    delay(250);
  }
}

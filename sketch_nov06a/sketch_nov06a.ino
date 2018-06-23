#include "Enes100.h"
#include "math.h"

#define THETA_ERROR .025
#define ROCKY_TERRAIN 1.1
#define MIDDLE_Y 2
#define US_OBSTACLE_DISTANCE_LIMIT 160
#define IR_OBSTACLE_DISTANCE_LIMIT 430
#define POS_ERROR 0.05
#define DESIRED_DIST 0.03
#define AIR_THRESH 100
#define FRESH_THRESH 300
#define POLL_THRESH 500
#define SALT_THRESH 700
#define TOUCH_THRESH 800

Enes100 enes("Team Oasis", WATER, 100, 8, 9);

float destinationX;
float destinationY;

int mr_on = 0; //right motor on (HIGH/LOW), ENA
int mr_for = 5; //right motor forward value, IN1
int mr_bck = 6; //right motor backward value, IN2
int ml_on = 1; //right motor on (HIGH/LOW), ENB
int ml_for = 10; //right motor forward value, IN3
int ml_bck = 11; //right motor backward value, IN4

int servoPin = 3;
int usTrigPin = 12;
int usEchoPin = 13;
int salPin = A1;
int irPin = A0;

void setup() {
  setupPins();
//  pinMode(LED_BUILTIN, OUTPUT);
  while (true){
    Serial.println(blockedByObstacle());
    Serial.println(readUS());
//    if (blockedByObstacle()){
//      digitalWrite(LED_BUILTIN,HIGH);
//    } else {
//      digitalWrite(LED_BUILTIN,LOW);
//    }
    delay(100);
  }

  delay(3000);

  setupPins();
  setupMotors();
  
  while (!enes.retrieveDestination()) {Serial.println("Can't get destination");}
  while (!enes.updateLocation()) {Serial.println("Can't get location");}
  
  setupDestination();
  
  navigationAlgorithm();
  setupPins();
}

void loop() {
}

void setupPins() {
  Serial.begin(9600);
  pinMode(usTrigPin, OUTPUT);
  pinMode(usEchoPin, INPUT);
  pinMode(servoPin, OUTPUT);
  analogWrite(servoPin, 0);
}

void setupMotors() {
  //setup pins
  pinMode(mr_on, OUTPUT);
  pinMode(mr_for, OUTPUT);
  pinMode(mr_bck, OUTPUT);
  pinMode(ml_on, OUTPUT);
  pinMode(ml_for, OUTPUT);
  pinMode(ml_bck, OUTPUT);

  stopMoving();
}

void setupDestination() {
  destinationX = enes.destination.x;
  destinationY = enes.destination.y;
}

void navigationAlgorithm() {
  rotatePerpendicularRockyTerrain();

  moveForwardFast();
  while (!pastTheRockyTerrain()) {}

  rotateTowardsMissionSite();
  moveForwardFast();

  while (!atMissionSite()) {
    if (blockedByObstacle()) {
      stopMoving();
      navigateAroundObstacle();
      rotateTowardsMissionSite();
      moveForwardFast();
    }
  }
  stopMoving();
}

float getCurrentX() {
  enes.updateLocation();
  return enes.location.x;
}

float getCurrentY() {
  enes.updateLocation();
  return enes.location.y;
}

float getCurrentAngle() {
  enes.updateLocation();
  return enes.location.theta;
}

float getDestinationAngle() {
  float x = destinationX - getCurrentX();
  float y = destinationY - getCurrentY();
  if (x == 0) {
    if (y > 0) return M_PI / 2;
    else return 3 * M_PI / 2;
  }
  float t = atan(y / x);
  if (x < 0) return t + M_PI;
  else if (y < 0) return t + 2 * M_PI;
  else return t;
}

float getPowerToTurn(float delta_t) {
  return 255 - 95 * pow(1 - delta_t / M_PI, 4);
}

void rotateToAngle(float angleToRotateTo) {
  stopMoving();
  float delta_t = formatAngle(getCurrentAngle() - angleToRotateTo);
  while (delta_t > THETA_ERROR && delta_t < 2 * M_PI - THETA_ERROR) {
    if (delta_t >= M_PI) turnCCW(getPowerToTurn(delta_t));
    else turnCW(getPowerToTurn(delta_t));
    delta_t = formatAngle(getCurrentAngle() - angleToRotateTo);
  }
  stopMoving();
}

float formatAngle(float angle) {
  return ((int)(1000 * (angle + 2 * M_PI)) % (2 * 3141)) / 1000.0;
}

void rotatePerpendicularRockyTerrain() {
  rotateToAngle(0);
}

void rotateTowardsMissionSite() {
  rotateToAngle(getDestinationAngle());
}

void turnCW(int pwr) {
  //right wheels backward
  digitalWrite(mr_on, HIGH);
  analogWrite(mr_for, 0);
  analogWrite(mr_bck, pwr);

  //left wheels forward
  digitalWrite(ml_on, HIGH);
  analogWrite(ml_for, pwr);
  analogWrite(ml_bck, 0);
}

void turnCCW(int pwr) {
  //right wheels forward
  digitalWrite(mr_on, HIGH);
  analogWrite(mr_for, pwr);
  analogWrite(mr_bck, 0);

  //left wheels backward
  digitalWrite(ml_on, HIGH);
  analogWrite(ml_for, 0);
  analogWrite(ml_bck, pwr);
}

void moveForwardFast() {
  moveForwardAux(255);
}

void moveForwardSlow() {
  moveForwardAux(128);
}

void moveForwardAux(int pwr) {
  //Right wheels
  digitalWrite(mr_on, HIGH);
  analogWrite(mr_for, pwr);
  analogWrite(mr_bck, 0);

  digitalWrite(ml_on, HIGH);
  analogWrite(ml_for, pwr);
  analogWrite(ml_bck, 0);
}

void stopMoving() {
  digitalWrite(mr_on, LOW);
  analogWrite(mr_for, 0);
  analogWrite(mr_bck, 0);
  digitalWrite(ml_on, LOW);
  analogWrite(ml_for, 0);
  analogWrite(ml_bck, 0);
}

boolean pastTheRockyTerrain() {
  return getCurrentX() > ROCKY_TERRAIN;
}

boolean blockedByObstacle() {
  return readIR() > 350 || readUS() < 160;
}

int readIR() {
  return analogRead(irPin);
}

int readUS() {
  digitalWrite(usTrigPin, LOW);
  delayMicroseconds(10);
  digitalWrite(usTrigPin, HIGH);
  delayMicroseconds(10);
  int duration = pulseIn(usEchoPin, HIGH);
  if (duration < 0) return 10000;
  return (duration / 2) / 2.91;
}

void navigateAroundObstacle() {
  if (getCurrentY() > MIDDLE_Y) {
    rotateToAngle(3 * M_PI / 2);
    moveForwardFast();
    delay(1500);
  } else {
    rotateToAngle(3 * M_PI / 2);
    moveForwardFast();
    delay(1000);
  }
}

void mission() {

  int depth = 0;
  bool baseDone = false;

  int sal = salinity();
  while (sal < TOUCH_THRESH) {
    lowerArm(1);
    sal = salinity();

    if (baseDone) {
      depth = depth + 1;
    } else {
      if (sal > AIR_THRESH) {
        lowerArm(5);
        depth = 5;
        if (sal > SALT_THRESH) {
          enes.baseObjective(SALT);
        } else if (sal > POLL_THRESH) {
          enes.baseObjective(POLLUTED);
        } else if (sal > FRESH_THRESH) {
          enes.baseObjective(FRESH);
        } else {
          enes.println("WE SUCK!");
          enes.baseObjective(FRESH);
        }
        baseDone = true;
        return;
      }
    }
  }
  enes.bonusObjective(depth);

}
void lowerArm(int dist) {
  int t = dist * 41.32;
  analogWrite(servoPin, -50);
  delay(t);
  analogWrite(servoPin, 0);
}

void raiseArm(int dist) {
  int t = dist * 41.32;
  analogWrite(servoPin, 50);
  delay(t);
  analogWrite(servoPin, 0);
}

int salinity() {
  int sum = 0;
  for (int i = 1; i < 10; i += 1) {
    delay(50);
    sum += analogRead(salPin);
  }
  return sum / 10;
}

bool atMissionSite() {
  float distToSite = (getCurrentX() - destinationX) * (getCurrentX() - destinationX) + (getCurrentY() - destinationY) * (getCurrentY() - destinationY);
  return distToSite - DESIRED_DIST * DESIRED_DIST < POS_ERROR * POS_ERROR;
}


#define ROCKY_TERRAIN_X_END 3.0
#define MISSION_X 3.0
#define MISSION_Y 4.0

void setup() {
  
}

void loop() {
   
}

void navigationAlgorithm(){
  rotatePerpendicularRockyTerrain();
  
  moveForward();
  while (!pastTheRockyTerrain(getCurrentX()){}
  stopMoving();

  while (!atMissionSite(getCurrentX(),getCurrentY())){
    if (!facingMissionSite()){
      rotateTowardsMissionSite();
    }
    
  }


}

float getCurrentX(){
  return 0;
}

float getCurrentY(){
  return 0;
}

void rotateToAngle(float angleToRotateTo){
  
}

void rotatePerpendicularRockyTerrain(){
  rotateToAngle(0);
}

void rotateTowardsMissionSite(){
  rotateToAngle(0);
}

void moveForward(){
  
}

void stopMoving(){
  
}

boolean facingMissionSite(){
  
}

boolean pastTheRockyTerrain(float currentX){
  return currentX > ROCKY_TERRAIN_X_END;
}

boolean atMissionSite(float currentX, float currentY){

  return true;
}

boolean blockedByObstacle(float irSensor1Distance, float irSensor2Distance){

  return true;
}

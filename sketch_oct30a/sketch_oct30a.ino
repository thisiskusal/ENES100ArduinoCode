#include "Enes100.h"

Enes100 enes("Team Oasis", WATER, 3, 8, 9);

void setup() {
  while (!enes.retrieveDestination()) {
    enes.println("Unable to retrieve location");
  }
}

void loop() {
  if (enes.updateLocation()) {
        enes.println("Huzzah! Location updated!");
        enes.print("My x coordinate is ");
        enes.println(enes.location.x);
        enes.print("My y coordinate is ");
        enes.println(enes.location.y);
        enes.print("My theta is ");
        enes.println(enes.location.theta);
    } else {
        enes.println("Sad trombone... I couldn't update my location");
    }
}

#include <Arduino.h>
#include <stdio.h>
#include <serial_interface.h>
#include <neopixel_blit.h>

/* Makes all the lights red */

Pixels pix;
int on_pixel = 0;
int cycle_speed = 100;

int left_input = 4;
int right_input = 19;

bool left_pressed = false;
bool right_pressed = false;

void setup(void) {
   init_serial_stdio();
   init_neopixel_blit();
   pinMode(right_input,INPUT);
   pinMode(left_input,INPUT);
}

void loop(void) {
  if (on_pixel == 0){
    printf("delay: %dms\n",cycle_speed);
  }
  
  for (int i = 0; i < 10; i++){
    if (i != on_pixel){
      pix[i][PIX_RED] = 0;
      pix[i][PIX_GRN] = 0;
      pix[i][PIX_BLU] = 0;
    } else {
      pix[i][PIX_RED] = 0;
      pix[i][PIX_GRN] = 15;
      pix[i][PIX_BLU] = 15;
    }
  }
  on_pixel = (on_pixel + 1) % 10;
  neopixel_blit(pix);

  if (digitalRead(left_input) == 1){
    if (!left_pressed){
      cycle_speed *= 2;
      left_pressed = true;
    }
  } else {
    left_pressed = false;
  }
  
  if (digitalRead(right_input) == 1){
    if (!right_pressed){
      cycle_speed /= 2;
      right_pressed = true;
    }
  } else {
    right_pressed = false;
  }
  
  if (cycle_speed > 1000){
      cycle_speed = 1000;
  } else if (cycle_speed < 2){
    cycle_speed = 2;
  }

  delay(cycle_speed);
}

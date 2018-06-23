#include <Arduino.h>
#include "neopixel_blit.h"

#ifndef CPLAY_NEOPIXELPIN
#define CPLAY_NEOPIXELPIN 17
#endif

volatile uint8_t *port;
uint8_t pinMask;

void init_neopixel_blit() {
  Pixels blank = { { 0 } };
  pinMode(CPLAY_NEOPIXELPIN, OUTPUT);
  digitalWrite(CPLAY_NEOPIXELPIN, LOW);
  /* determine which port is associated with pin "D17" (=>
     Port B), then find the register associated with that
     port */
  port    = portOutputRegister(digitalPinToPort(CPLAY_NEOPIXELPIN));
  /* pinMask = _BV(0), because the neopixels are on PB0 */ 
  pinMask = digitalPinToBitMask(CPLAY_NEOPIXELPIN);
  neopixel_blit(blank);
}

/* nspring copied from circuit playground library, altered
   slightly to be more C, removed neopixel/arduino
   documentation comments, made specific to the circuit
   playground (hardcoded constants, omission of #ifdef for
   alternate architectures), and rewrote some comments. */
void neopixel_blit(const void *pixels) {
  static unsigned int endTime;
  volatile uint16_t i = 30; 
  volatile const uint8_t *ptr = (const uint8_t *)pixels;
  volatile uint8_t
    b   = *ptr++,   // Current byte value
    hi,             // PORT w/output bit set high
    lo;             // PORT w/output bit set low

  volatile uint8_t n1, n2 = 0;  // First, next bits out
  
  /* this was while(canShow()); ensures we're 50us after the
     last call to neopixel_blit().  A semicolon on an line
     by itself emphasizes that this while() loop has no
     body. */
  while(micros() - endTime < 50)
    ;
  
  /* disable interrupts, ensuring that our timing is not,
     well, interrupted */
  noInterrupts();
  
  /* nspring doesn't properly understand this stanza. */
  hi = PORTB |  pinMask;
  lo = PORTB & ~pinMask;
  n1 = lo;
  if(b & 0x80) n1 = hi;

      asm volatile(
       "cp_headB:"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 6"        "\n\t"
         "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 5"        "\n\t"
         "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 4"        "\n\t"
         "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 3"        "\n\t"
         "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 2"        "\n\t"
         "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 1"        "\n\t"
         "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n2]   , %[lo]"    "\n\t"
        "out  %[port] , %[n1]"    "\n\t"
        "rjmp .+0"                "\n\t"
        "sbrc %[byte] , 0"        "\n\t"
         "mov %[n2]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "sbiw %[count], 1"        "\n\t"
        "out  %[port] , %[hi]"    "\n\t"
        "mov  %[n1]   , %[lo]"    "\n\t"
        "out  %[port] , %[n2]"    "\n\t"
        "ld   %[byte] , %a[ptr]+" "\n\t"
        "sbrc %[byte] , 7"        "\n\t"
         "mov %[n1]   , %[hi]"    "\n\t"
        "out  %[port] , %[lo]"    "\n\t"
        "brne cp_headB"           "\n"
      : [byte] "+r" (b), [n1] "+r" (n1), [n2] "+r" (n2), [count] "+w" (i)
      : [port] "I" (_SFR_IO_ADDR(PORTB)), [ptr] "e" (ptr), [hi] "r" (hi),
        [lo] "r" (lo));

      /* enable interrupts */
      interrupts();

      /* set the end time of this neopixel_blit() so that we can delay the next one if necessary */
      endTime = micros();
}

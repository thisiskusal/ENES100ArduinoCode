/* The following code permits the use of printf() from
   Arduino source by setting stdin/stdout FILE pointers to
   use the default Serial device (for the circuit
   playground, the USB connection) */

/* synthesized from http://playground.arduino.cc/Main/Printf
   with initial draft from
   https://github.com/tuupola/avr_demo/blob/master/hello_uart/uart.c */

/* See also,
   http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html
   which enumerates the stdio functions supported */

#include <stdio.h>   /* for fdev_ stuff */
#include <Arduino.h> /* for Serial */
#include "serial_interface.h"

/* not used outside this module */
static FILE serial_output;
static FILE serial_input;

/* not to be called from outside this module: use putchar or
   getchar instead if necessary or printf / gets / scanf. */
static int serial_stdio_putchar(char c, FILE *stream);
static int serial_stdio_getchar(FILE *stream);

void init_serial_stdio(void) {
  fdev_setup_stream(&serial_output, serial_stdio_putchar, NULL, _FDEV_SETUP_WRITE);
  fdev_setup_stream(&serial_input, NULL, serial_stdio_getchar, _FDEV_SETUP_READ);
  stdout = &serial_output;
  stdin = &serial_input;
  
  USBDevice.attach(); /* called by arduino main(), but make sure. */
  Serial.begin(9600);
}

static int serial_stdio_putchar(char c, FILE *stream) {
  if (c == '\n') {
    Serial.write('\r');
  }
  Serial.write(c);

  /* for the simulator */
  if (c == '\n') {
    loop_until_bit_is_set(UCSR1A, UDRE1);
    UDR1 = '\r';
  }
  loop_until_bit_is_set(UCSR1A, UDRE1);
  UDR1 = c;
  /* end simulator */
  
  return 0;
}

/* The following getchar implementation (a) blocks
   until a character is received, which could be bad, 
   but makes sense, and (b) converts incoming \r's into
   \n's, which seemed necessary to free fgets() to return
   the string. */
static int serial_stdio_getchar(FILE *stream) {
  /* blocking; translating \n into \r */
  int c;

  do { /* repeat until we get a character */
    c = Serial.read();
  } while(c < 0);

  Serial.write(c); /* echo */

  return ( c == '\r' ? '\n' : c );
}



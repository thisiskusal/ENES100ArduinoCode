/* Invoking init_serial_stdio() permits using printf() and
   getchar() (at least) from the AVR library, enabling C
   standard interactions. */

/* written by nspring for cmsc216/h Jan 2017, updated Jan 2018 */

/* allows inclusion by C++ code. */
#ifdef __cplusplus
extern "C" {
#endif

  /* call this in setup().  It ensures that printf and scanf
     use the (virtual) serial port for input and output.  */
  void init_serial_stdio(void);

#ifdef __cplusplus
} /* end extern "C" */
#endif


/* This is a symbol that, if set, represents a call that
   should be made frequently to poll the serial device.
   If using setup/loop, this is called automatically by
   arduino's main() function.  If writing your own main(),
   this should be in the loop. */
/* "weak" tells the linker that the function may not be
   implemented, and not to complain.  It says that the
   programmer will test whether it is set before calling
   it.  That is, "if(serialEventRun) serialEventRun();" */
/* it may be declared with C++ linkage. */
extern void serialEventRun(void) __attribute__((weak));



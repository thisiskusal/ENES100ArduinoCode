#include <stdint.h>

typedef uint8_t Pixels[10][3];
typedef struct Pixel { 
  uint8_t green, red, blue;
} PixelStruct[10];

#define PIX_GRN 0
#define PIX_RED 1
#define PIX_BLU 2

#ifdef __cplusplus
extern "C" {
#endif

  void init_neopixel_blit();
  void neopixel_blit(const void *pixels); /* typically, Pixels or PixelStruct */

#ifdef __cplusplus
}
#endif

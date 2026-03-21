#include "bounce_animation.h"
#include <MD_MAX72xx.h>

int x = 0, y = 0;
int dirX = 1, dirY = 1;

// This "extern" tells the compiler the 'matrix' object is defined elsewhere
extern MD_MAX72XX matrix;

void playBounceAnimation() {
  matrix.setPoint(y, x, false); // Erase old

  x += dirX;
  y += dirY;

  if (x >= 31 || x <= 0) dirX *= -1;
  if (y >= 7 || y <= 0) dirY *= -1;

  matrix.setPoint(y, x, true); // Draw new
  delay(60);
}
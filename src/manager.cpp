#include<manager.h>
#include <MD_MAX72xx.h>

void playBounceAnimation();
void playMatrixAnimation();
// void updateSnake();

// Shared hardware object
extern MD_MAX72XX matrix;

void AppManager::setMode(AppMode mode) {
  matrix.clear();
  currentMode = mode;
  
  switch (mode) {
    case MATRIX:  updateFunc = playMatrixAnimation;  updateInterval = 1000; break;
    case BOUNCE: updateFunc = playBounceAnimation; updateInterval = 50;   break;
    // case SNAKE:  updateFunc = updateSnake;  updateInterval = 150;  break;
  }
}

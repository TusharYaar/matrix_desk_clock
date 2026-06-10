#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

// Define which mode the ESP32 is in
enum AppMode {
  CLOCK = 0,
  MATRIX,
  BOUNCE,
  RANDOM,
  MODE_COUNT
};
// SNAKE,

struct AppManager {
  AppMode currentMode;
  unsigned long lastUpdate;
  int updateInterval;

  // Function pointers to wrap our different project logics
  void (*updateFunc)(); 
  
  void setMode(AppMode mode);
};



#endif

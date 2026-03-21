#ifndef CLOCK_H
#define CLOCK_H

#define COLUMNS 32


void initializeClock();
// Function declaration
void showClock();
void animateTime();
void setTime(int hour, int minute, int second, bool shift = true);

#endif
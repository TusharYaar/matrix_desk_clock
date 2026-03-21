#include<matrix_animation.h>
#include<MD_MAX72xx.h>
#include <math.h>

extern MD_MAX72XX matrix;

const int diff = 4;

uint16_t columns[COLUMNS];


bool defined = false;
uint8_t shift;

void playMatrixAnimation() {
    if (!defined) {
        setColumnValues();
        defined=true;
    }
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // 1. "Lock" the screen
    matrix.clear();
    for (int i=0; i < COLUMNS;i++) {
        shift = columns[i] >> 1;
        matrix.setColumn(i, shift);
        columns[i] = columns[i] >> 1;
        if (shift < 128) {
            columns[i] += random(255,1025) * 256;
        }
    
    }
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(200);

}


void setColumnValues() {
for (int i=0;i<COLUMNS;i++) {
    columns[i] = random(1025);
 }
}


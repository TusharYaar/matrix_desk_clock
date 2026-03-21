#include<random_animation.h>
#include<MD_MAX72xx.h>
#include <math.h>

extern MD_MAX72XX matrix;

const int random_points = 50;

Point points[random_points];

void playRandomAnimation() {
    setRandomPoints();
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // 1. "Lock" the screen
    matrix.clear();
    for (int i=0; i < random_points;i++) {
        matrix.setPoint(points[i].x,points[i].y, true);
    }
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(60);

}


void setRandomPoints() {
    Point point;
    for (int i =0;i < random_points;i++) {
        point.y = random(X_LIMIT);
        point.x = random(Y_LIMIT);
        points[i] = point;    
    }
}


#ifndef SNAKE_H
#define SNAKE_H

#define X_LIMIT 32
#define Y_LIMIT 8

struct Point2 {
    int x;
    int y;
};

struct Snake
{   int x;
    int y;
    struct Snake* next;
};

// Function declaration
void setFood();
void moveSnake();
void handleSnakeGame();
void turnSnake(int dir);


#endif
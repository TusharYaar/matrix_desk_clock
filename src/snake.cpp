#include "snake.h"
#include <MD_MAX72xx.h>

extern MD_MAX72XX matrix;
int snake_dirX = 0, snake_dirY = 1;

#define COLUMNS 32

Point2 food = {
    x : 3,
    y : 15,
};

Snake head = {
    x : 3,
    y : 5,
    next : nullptr,
};

int SPEED = 400;
void setFood()
{
    while (true)
    {
        bool overlapping = false;
        int x = 3; // random(0, 8);
        int y = random(0, COLUMNS);
        Snake *ptr = &head;
        while (ptr != nullptr)
        {
            if (ptr->x == x && ptr->y == y)
            {
                overlapping = true;
                break;
            }
            ptr = ptr->next;
        }

        Serial.printf("SETTING FOOD AT %d %d \n", x, y);

        if (!overlapping)
        {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

void turnSnake(int dir)
{
    if (snake_dirX != 0)
    {
        snake_dirY = dir;
        snake_dirX = 0;
    }
    else if (snake_dirY != -1)
    {
        snake_dirX = dir;
        snake_dirY = 0;
    }
}

void handleSnakeGame()
{
    moveSnake();
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // 1. "Lock" the screen
    matrix.clear();
    Snake *ptr = &head;
    while (ptr != nullptr)
    {
        matrix.setPoint(ptr->x, ptr->y, true);
        ptr = ptr->next;
    }

    matrix.setPoint(food.x, food.y, true);
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(SPEED);
}

void moveSnake()
{
    int last_x = head.x;
    int last_y = head.y;
    Snake *ptr = head.next;
    head.x = head.x + snake_dirX;
    head.y = head.y + snake_dirY;
    if (head.y > COLUMNS)
    {
        head.y = 0;
    }
    bool foundFood = false;
    if (head.x == food.x && head.y == food.y)
    {
        Serial.println("Setting FOOD");
        foundFood = true;
        setFood();
    }

    while (ptr != nullptr)
    {
        int currx = ptr->x;
        int curry = ptr->y;
        ptr->x = last_x;
        ptr->y = last_y;
        last_x = currx;
        last_y = curry;

        if (ptr->next == nullptr)
        {
            if (foundFood)
            {
                Snake tail = {
                    x : last_x,
                    y : last_y,
                    next : nullptr,
                };
                ptr->next = &tail;
            }
            break;
        }
        ptr = ptr->next;
    }
}

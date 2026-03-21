#include <NTPClient.h>
#include <clock.h>
#include <MD_MAX72xx.h>
#include <WiFi.h>
#include <fonts.h>
#include <WiFiUdp.h>
#include <time.h>

extern MD_MAX72XX matrix;
uint16_t columns_clock[COLUMNS];
uint16_t columns_clock_shift[COLUMNS] = {0};

const long utcOffsetInSeconds = 19800;
// WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds, 60000);

int last_second = 00;
char last_time_str[] = "00:00:00";
char time_str[] = "00:00:00";


int padding = 2;
struct tm timeinfo;


void initializeClock()
{
    configTime(0, utcOffsetInSeconds, "asia.pool.ntp.org");
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
    }
    else
    {
        setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        last_second = timeinfo.tm_sec;
    }
    for (int i = 0; i < padding; i++)
    {
        columns_clock[0] = 0;
    }
}

void showClock()
{
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
    }

    if (timeinfo.tm_sec != last_second)
    {
        setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        last_second = timeinfo.tm_sec;
    }
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF); // 1. "Lock" the screen
    matrix.clear();
    for (int i = 0; i < COLUMNS; i++)
    {   
        if (columns_clock_shift[i] != 0) {
            u16_t shift = columns_clock[i] >> 1;
            columns_clock[i] = shift;
            matrix.setColumn(i, shift);
            columns_clock_shift[i] = columns_clock_shift[i] - 1;
        }
        else {
            matrix.setColumn(i, columns_clock[i]);
        }
    }
    matrix.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
    delay(100);
}

void setTime(int hour, int minute, int second, bool shift)
{
    strcpy(last_time_str, time_str);
    time_str[1] = hour % 10 + 48;
    time_str[0] = hour / 10 + 48;
    time_str[4] = minute % 10 + 48;
    time_str[3] = minute / 10 + 48;
    time_str[7] = second % 10 + 48;
    time_str[6] = second / 10 + 48;

    animateTime();
}

void animateTime()
{

    int colIndex = padding;
    int animate_shift;
    for (int i = 7; i >= 0; i--)
    {
        animate_shift = 0;
        if (i != 7)
        {
            columns_clock[colIndex++] = 0;
        }
        if (time_str[i] == ':')
        {
            columns_clock[colIndex++] = 20;
            columns_clock[colIndex++] = 20;
        }
        else
        {
            byte number = time_str[i] - 48;
            for (int ii = 2; ii >= 0; ii--)
            {
                if (last_time_str[i] != time_str[i] && strcmp(last_time_str,"00:00:00") != 0) {
                    columns_clock[colIndex] = (SmallDigits[number][ii] * 256) + columns_clock[colIndex];
                    columns_clock_shift[colIndex++] = 8;
                }
                else 
                    columns_clock[colIndex++] = SmallDigits[number][ii];
                    
                
            }
        }
    }
}
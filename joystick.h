#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdio.h>

#define UP "/sys/class/gpio/gpio26/value"
#define DOWN "/sys/class/gpio/gpio46/value"
#define LEFT "/sys/class/gpio/gpio65/value"
#define RIGHT "/sys/class/gpio/gpio47/value"
#define IN "/sys/class/gpio/gpio27/value"

#define MAX_LENGTH 1024

void joystickControler_init(void);

void nsleep(long sec, long nanosec);

int readFromFileToScreen(char *fileName);
char getJoystickVal();

void* joystickThread(void* arg);
void joystickFree(void);

#endif
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "joystick.h"
#include "audioMixer_template.h"

#define JOYSTICK_GPIO_VALUE_UP 26
#define JOYSTICK_GPIO_VALUE_RIGHT 47
#define JOYSTICK_GPIO_VALUE_DOWN 46
#define JOYSTICK_GPIO_VALUE_LEFT 65
#define JOYSTICK_GPIO_VALUE_IN 27

#define GPIO_EXPORT_PATH "/sys/class/gpio/export"

static void pinExporter(int value);

pthread_t threadJoystick;

void nsleep(long sec, long nanosec){
	long seconds = sec;
	long nanoseconds = nanosec;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

void joystickControler_init(void) {
	pinExporter (JOYSTICK_GPIO_VALUE_UP);
	pinExporter (JOYSTICK_GPIO_VALUE_RIGHT);
	pinExporter (JOYSTICK_GPIO_VALUE_DOWN);
	pinExporter (JOYSTICK_GPIO_VALUE_LEFT);
	pinExporter (JOYSTICK_GPIO_VALUE_IN);
}

static void pinExporter(int gpioNum) {
	FILE *pfile = fopen(GPIO_EXPORT_PATH, "w");
	if (pfile == NULL) {
		printf("ERROR: Unable to open export file.\n");
		exit(1);
	}

	// Write to data to the file using fprintf():
	fprintf(pfile, "%d", gpioNum);

	// Close the file using fclose():
	fclose(pfile);
}

int readFromFileToScreen(char *fileName) {
    FILE *pFile = fopen(fileName, "r");
    if (pFile == NULL) {
        printf("ERROR: Unable to open file (%s) for read\n", fileName);
        exit(-1);
    }
    // Read string (line)
    //const int MAX_LENGTH = 1024;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);
    // Close
    fclose(pFile);
    int val = atoi(buff);
    return val;
}

char getJoystickVal() {
    char val = 0;

        int UpVal = readFromFileToScreen(UP);
        int DownVal = readFromFileToScreen(DOWN);
        int LeftVal = readFromFileToScreen(LEFT);
        int RightVal = readFromFileToScreen(RIGHT);
        int InVal = readFromFileToScreen(IN);

        if(UpVal == 0) {
            val = 'U';
//            printf("%c\n",val);
        }
        else if(DownVal == 0) {
            val = 'D';
//            printf("%c\n",val);
        }
        else if(LeftVal == 0) {
            val = 'L';
//            printf("%c\n",val);
        }
        else if(RightVal == 0) {
            val = 'R';
//            printf("%c\n",val);
        }
        else if(InVal == 0) {
            val = 'I';
//            printf("%c\n",val);
        }
    return val;
}

void* joystickThread(void* arg) {

    joystickControler_init();

	int offset = 5;

	while(1) {
		if (getJoystickVal() == 'U') {
			AudioMixer_setVolume(AudioMixer_getVolume() + offset);
			printf("VOL: %d\n", AudioMixer_getVolume());
		}else if (getJoystickVal() == 'D')  {
			AudioMixer_setVolume(AudioMixer_getVolume() - offset);
			printf("VOL: %d\n", AudioMixer_getVolume());
		}else if (getJoystickVal() == 'L') {
			AudioMixer_setBPM(AudioMixer_getBPM() + offset);
			printf("BPM: %d\n", AudioMixer_getBPM());
		}else if (getJoystickVal() == 'R') {
			AudioMixer_setBPM(AudioMixer_getBPM() - offset);
			printf("BPM: %d\n", AudioMixer_getBPM());
		}else if (getJoystickVal() == 'I') {
			AudioMixer_nextState();
			printf("State: %d\n", AudioMixer_getState());
		}
		nsleep(0, 100000000);
	}
	pthread_exit(0);
}

void joystickFree(void){
	pthread_join(threadJoystick, NULL);
}
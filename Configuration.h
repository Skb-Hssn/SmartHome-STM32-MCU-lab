#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Configuration {
	int temp;
	int water;
	int humidity;
	int light[4];
	int dayNight;
};

void parseConfiguration(char* conf, struct Configuration* configuration, char* retConfig);

void getNext(char ch[], char ret[], int* pos);


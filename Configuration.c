#include "Configuration.h"

void getNext(char ch[], char ret[], int* pos)
{
    int j = 0;
    while(ch[*pos] != 0 && ch[*pos] != ' ' && ch[*pos] != '.') {
        char c = ch[*pos];

        if(c >= 'A' && c <= 'Z') c = (char) (c - 'A' + 'a');

        if(j == 0 && c >= 'a' && c <= 'z') c = (char) (c - 'a' + 'A');

        ret[j++] = c;
        (*pos)++;
    }
    ret[j] = 0;
    (*pos)++;
}

void parseConfiguration(char* conf, struct Configuration* configuration, char retConfig[])
{
	// struct Configuration configuration;
	int len = 0;
	while(len < 150 && conf[len] != '.') len++;
    len++;

    int i = 0;

    char cur[10];
    getNext(conf, cur, &i);

    // printf("32: cur : %s\n", cur);
	
    if(strcmp(cur, "Config") == 0) {
        for(; i < len; ) {
            if(conf[i] == ' ' || conf[i] == '.') {
                i++; 
                continue;
            }
            char cur[10];
            getNext(conf, cur, &i);

            if(strcmp(cur, "Temp") == 0 || strcmp(cur, "Water") == 0 || strcmp(cur, "Humidity") == 0) {
                if(i < len) {
                    char val[10];
                    getNext(conf, val, &i);

                    int len2 = strlen(val);
                    int ok = 1;
                    int j = 0;

                    while(j < len2) {
                        if(!(val[j] >= '0' && val[j] <= '9')) {
                            ok = 0;
                        }
                        j++;
                    }

                    if(ok) {
                        if(strcmp(cur, "Temp") == 0) configuration->temp = atoi(val);
                        if(strcmp(cur, "Water") == 0) configuration->water = atoi(val);
                        if(strcmp(cur, "Humidity") == 0) configuration->humidity = atoi(val);
                    }
                }
            } else if(strcmp(cur, "Light") == 0) {
                if(i < len) {
                    char val[10];
                    getNext(conf, val, &i);

                    int len2 = (int)strlen(val);
                    int ok = 1;
                    int j = 0;

                    while(j < len2) {
                        if(!(val[j] >= '0' && val[j] <= '1') || strlen(val) < 4) {
                            ok = 0;
                        }
                        j++;
                    }

                    if(ok) {
                        for(int j = 0; j < 4; j++) {
                            configuration->light[j] = val[j] - '0';
                        }
                    }
                }
            } else if(strcmp(cur, "Day") == 0) {
                configuration->dayNight = 1;
            } else if(strcmp(cur, "Night") == 0) {
                configuration->dayNight = 0;
            }
        }
    } else if(strcmp(cur, "Read") == 0) {
        // printf("94 : Read\n");
        char ret[150];
        ret[0] = 0;
        strcat(ret, "Config ");

        int onlyConfig = 1;

        for(; i < len; ) {
            if(conf[i] == ' ' || conf[i] == '.') continue;
            char cur[10];
            getNext(conf, cur, &i);

            if(strcmp(cur, "Config")) {
                onlyConfig = 0;
            }
        }

        char temp[10];
        if(!onlyConfig) {
            for(i = 0; i < len; ) {
                getNext(conf, cur, &i);

                if(strcmp(cur, "Temp") == 0) {
                    strcat(ret, "Temp ");
                    sprintf(temp, "%d ", configuration->temp);
                    strcat(ret, temp);
                } else if(strcmp(cur, "Water") == 0) {
                    strcat(ret, "Water ");
                    sprintf(temp, "%d ", configuration->water);
                    strcat(ret, temp);
                } else if(strcmp(cur, "Humidity") == 0) {
                    strcat(ret, "Humidity ");
                    sprintf(temp, "%d ", configuration->humidity);
                    strcat(ret, temp);
                } else if(strcmp(cur, "Light") == 0) {
                    strcat(ret, "Light ");
                    sprintf(temp, "%d", configuration->light[0]);
                    strcat(ret, temp);
                    sprintf(temp, "%d", configuration->light[1]);
                    strcat(ret, temp);
                    sprintf(temp, "%d", configuration->light[2]);
                    strcat(ret, temp);
                    sprintf(temp, "%d ", configuration->light[3]);
                    strcat(ret, temp);
                } else if(strcmp(cur, "Read") == 0) {
                } else if(strcmp(cur, "Config") == 0) {
                } else {
                    i++;
                }
            }
        } else {
            strcat(ret, "Temp ");
            sprintf(temp, "%d ", configuration->temp);
            strcat(ret, temp);

            strcat(ret, "Water ");
            sprintf(temp, "%d ", configuration->water);
            strcat(ret, temp);

            strcat(ret, "Humidity ");
            sprintf(temp, "%d ", configuration->humidity);
            strcat(ret, temp);

            strcat(ret, "Light ");
            sprintf(temp, "%d", configuration->light[0]);
            strcat(ret, temp);
            sprintf(temp, "%d", configuration->light[1]);
            strcat(ret, temp);
            sprintf(temp, "%d", configuration->light[2]);
            strcat(ret, temp);
            sprintf(temp, "%d", configuration->light[3]);
            strcat(ret, temp);
        }

        i = 0;
        while(i < (int)strlen(ret)) retConfig[i] = ret[i], i++;
				retConfig[i] = '.';
    }
}

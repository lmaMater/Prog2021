#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int ToSeconds(int days, int hours, int minutes, int seconds) {
    return seconds + minutes * 60 + hours * 3600 + days * 86400;
}

int stringToSeconds(char *str) {
    int day, hour, min, sec;
    for (int i = 0; i < 1024; i++) {
        if (str[i] == '[') {
            int x = 0;
            x += i;
            char first[2] = {str[i + 1]}, second[2] = {str[i + 2]};
            strcat(first, second);
            i += x;
            char first2[2] = {str[i + 13]}, second2[2] = {str[i + 14]};
            strcat(first2, second2);
            i += x;
            char first3[2] = {str[i + 16]}, second3[2] = {str[i + 17]};
            strcat(first3, second3);
            i += x;
            char first4[2] = {str[i + 19]}, second4[2] = {str[i + 20]};
            strcat(first4, second4);
            day = atoi(first);
            hour = atoi(first2);
            min = atoi(first3);
            sec = atoi(first4);
            break;
        }
    }
    ToSeconds(day, hour, min, sec);
}

void outErrors(char c) {
    char line[1024];
    char *str;
    FILE *input = fopen("access_log.txt", "r");
    str = fgets(line, 1024, input);
    while (str != NULL) {
        int flag = 0;
        for (int i = 0; i < 1024; i++) {
            if (str[i] == '\"' && flag == 1) {
                if (str[i + 2] == c) {
                    printf("%s", str);
                    break;
                } else {
                    break;
                }
            } else if (str[i] == '\"' && flag == 0) {
                flag += 1;
            }
        }
        str = fgets(line, 1024, input);
    }
    fclose(input);
}

int main() {
    // out lines with 5xx - error
    outErrors('5');

    // out defined window with maximum requests
    int dayTimeWindow, hourTimeWindow, minTimeWindow, secTimeWindow;
    printf("\nEnter the number of days hours minutes seconds\n"
           "of the time window, separated by a space:\n");
    scanf("%d %d %d %d", &dayTimeWindow, &hourTimeWindow, &minTimeWindow, &secTimeWindow);

    int timeWindow = secTimeWindow + minTimeWindow * 60
                     + hourTimeWindow * 3600 + dayTimeWindow * 86400;
    char line[1024];
    char *str;

    FILE *input = fopen("access_log.txt", "r");
    str = fgets(line, 1024, input);
    int curSeconds = stringToSeconds(str);
    int firstTimeMaxCount = 0, lastTimeMaxCount = 0, maxCount = 0,
            countElementInArr = -1, arrLength = 1024;
    int count = 0, firstTime = curSeconds,
            lastTime = curSeconds, indexFirstElementInArr = 0;
    int *arr = (int *) malloc(arrLength * sizeof(int));
    while (str != NULL) {
        if (countElementInArr + 1 >= arrLength - 1) {
            arrLength *= 2;
            arr = realloc(arr, arrLength * sizeof(int));
        }
        if (curSeconds - firstTime <= timeWindow) {
            countElementInArr += 1;
            count += 1;
            arr[countElementInArr] = curSeconds;
            lastTime = curSeconds;
        } else {
            if (count > maxCount) {
                maxCount = count;
                firstTimeMaxCount = firstTime;
                lastTimeMaxCount = lastTime;
            }
            if (count > 1) {
                count -= 1;
                indexFirstElementInArr += 1;
                firstTime = arr[indexFirstElementInArr];
                while (curSeconds - firstTime > timeWindow && count > 1) {
                    count -= 1;
                    indexFirstElementInArr += 1;
                    firstTime = arr[indexFirstElementInArr];
                }
                if (curSeconds - firstTime <= timeWindow && count > 0) {
                    count += 1;
                    lastTime = curSeconds;
                    countElementInArr += 1;
                    arr[countElementInArr] = curSeconds;
                } else if (curSeconds - firstTime > timeWindow && count == 1) {
                    indexFirstElementInArr += 1;
                    firstTime = curSeconds;
                    lastTime = curSeconds;
                    countElementInArr += 1;
                    arr[countElementInArr] = curSeconds;
                }
            } else {
                indexFirstElementInArr += 1;
                firstTime = curSeconds;
                lastTime = curSeconds;
                countElementInArr += 1;
                arr[countElementInArr] = curSeconds;
            }

        }

        str = fgets(line, 1024, input);
        if (str != NULL) {
            curSeconds = stringToSeconds(str);
        } else {
            if (count > maxCount) {
                maxCount = count;
                firstTimeMaxCount = firstTime;
                lastTimeMaxCount = lastTime;
            }
            break;
        }
    }
    
    printf("Maximum request number: %d\n", maxCount);
    
    int dayLast, hourLast, minLast, secLast;
    int dayFirst, hourFirst, minFirst, secFirst;
    
    secFirst = firstTimeMaxCount % 60;
    firstTimeMaxCount /= 60;
    minFirst = firstTimeMaxCount % 60;
    firstTimeMaxCount /= 60;
    hourFirst = firstTimeMaxCount % 60;
    firstTimeMaxCount /= 24;
    dayFirst = firstTimeMaxCount % 24;
    printf("First Time: 1995 %d Jul %d:%d:%d\n", 
           dayFirst, hourFirst % 24,
           minFirst % 60, secFirst % 60);
    
    secLast = lastTimeMaxCount % 60;
    lastTimeMaxCount /= 60;
    minLast = lastTimeMaxCount % 60;
    lastTimeMaxCount /= 60;
    hourLast = lastTimeMaxCount % 60;
    lastTimeMaxCount /= 24;
    dayLast = lastTimeMaxCount % 24;
    printf("Last Time: 1995 %d Jul %d:%d:%d\n", 
           dayLast, hourLast % 24,
           minLast % 60, secLast % 60);
    
    free(arr);
    
    return 0;
}
#include <stdio.h>
#include <time.h>

int main (void) {
    time_t t;
    time(&t);
    printf("Current local time and date: %s",ctime(&t));
}
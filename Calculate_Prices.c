#include <stdio.h>

int main (void) {
    FILE *fp;
    char str[8];
    
    fp =  fopen("files.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }
    if (fgets(str, 60))
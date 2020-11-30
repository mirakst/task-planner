#include <stdio.h>
#include <stdlib.h>
#include "calculate_prices.h"

/* Returns price of hours sorted, as double values. */
int Calculate_Prices (double prices[][2], int sort, int day) {
    FILE *fp = fopen(PRICES_FILE, "r");

    if (fp == NULL) {
        printf("Error opening file '%s'.\n", PRICES_FILE);
        return -1;
    }

    Load_Prices (fp, prices, day);
    if (sort)
        qsort(prices, HOURS_PER_DAY, 2*(sizeof(double)), Double_Compare);

    return 1;
}

/* A function that fetches 24 lines worth of prices from file fp, 
 * then assigns them to numbered 2d array */
void Load_Prices (FILE *fp, double prices[][2], int day) {
    int i,
        k;
    double price;

    fscanf(fp, "%*[^ ]");
    for (k = 1; k < day; k++) {
        fscanf(fp, "%*[^\n]");
        fscanf(fp, "%*[^ ]");
    }
    for (i = 0; i < HOURS_PER_DAY; i++) {
        fscanf(fp, "%lf;", &price);
        /* printf("[DEBUG] The price fetched is: %s"); */
        prices[i][1] = i;
        prices[i][0] = price;
    }
}

/* A comparing function for qsort, typecasting input to doubles */
int Double_Compare (const void *x, const void *y) {
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) 
        return -1;
  if (xx > yy) 
        return  1;
  return 0;
}

void List_Prices (double prices[][2]) {
    int i;
    for(i = 0; i < 24; i++)
        printf("[%.2d - %.2d] is priced at %.2f.\n", 
                (int) prices[i][1], ((int) prices[i][1] + 1), prices[i][0]);
}

void Change_Day (double prices[][2]) {
    int day;
    char temp_input[MAX_INPUT];
    printf("Please enter the day you wish to load prices from: ");
    fgets (temp_input, MAX_INPUT, stdin);
    sscanf(temp_input, "%d", &day);
    if (day > 365) {
        printf("Day is too high. Changed day to %d.\n", day % 365);
        day = day % 365;
    }
    else if (day == 0) {
        day = 1;
        printf("Day can't be 0 or below. Changed day to 1.\n");
    }
    Calculate_Prices (prices, 0, day);
    printf("Prices have been initialized from day: %d.\n", day);
}
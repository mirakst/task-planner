#include <stdio.h>
#include <stdlib.h>
#include "calculate_prices.h"

#define HOURS_PR_DAY 24
#define PRICES_FILE "prices.txt"


/* Returns price of hours sorted, as double values. */
void Calculate_Prices (double prices[][2]) {
    char str[13];
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file '%s'.\n", filename);
        return;
    }

    Load_Prices (str, fp, prices);
    qsort(prices, HOURS_PR_DAY, 2*(sizeof(double)), Double_Compare);
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

/* A function that fetches 24 lines worth of prices from file fp, 
 * then assigns them to numbered 2d array */
void Load_Prices (char* str, FILE *fp, double prices[][2]) {
    int i;
    for (i = 0; i < HOURS_PR_DAY; i++) {
        fgets(str, 13, fp);
        /* printf("[DEBUG] The price fetched is: %s", str); */
        prices[i][1] = i;
        prices[i][0] = strtod(str, NULL);
    }
}

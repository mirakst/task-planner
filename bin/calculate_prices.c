#include <stdio.h>
#include <stdlib.h>
#include "calculate_prices.h"

/* Returns price of hours sorted, as double values. */
int Calculate_Prices (double prices[][2], int sort) {
    char str[13];
    FILE *fp = fopen(PRICES_FILE, "r");

    if (fp == NULL) {
        printf("Error opening file '%s'.\n", PRICES_FILE);
        return -1;
    }

    Load_Prices (str, fp, prices);
    if (sort)
        qsort(prices, HOURS_PER_DAY, 2*(sizeof(double)), Double_Compare);

    return 1;
}

/* A function that fetches 24 lines worth of prices from file fp, 
 * then assigns them to numbered 2d array */
void Load_Prices (char* str, FILE *fp, double prices[][2]) {
    int i;
    for (i = 0; i < HOURS_PER_DAY; i++) {
        fgets(str, 13, fp);
        /* printf("[DEBUG] The price fetched is: %s", str); */
        prices[i][1] = i;
        prices[i][0] = strtod(str, NULL);
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
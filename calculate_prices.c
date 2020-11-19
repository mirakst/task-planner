/* main needs to be renamed to Calculate_Prices to be properly integrated in main program.
 * Instead of void, main will take pointer arguments for the file and the array. */

#include <stdio.h>
#include <stdlib.h>

#define HOURS_PR_DAY 24

/* Prototypes */
int Double_Compare (const void *, const void *);
void Load_Prices (char*, FILE *fp, double prices[][2]);

/* Return type is double** so it can return the 2d array prices */
double** main (void) {
    FILE *fp = fopen("prices.txt", "r");
    char str[13];
    int i;
    double prices[HOURS_PR_DAY][2];

/* Error message in case the file doesn't load correctly */
    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    Load_Prices (str, fp, prices);

    for (i = 0; i < HOURS_PR_DAY; i++) {
        printf("[DEBUG] The price stored in slot [%d] is: %.2f\n", i, prices[i][0]);
    }
    printf("\n");
    
    qsort(prices, HOURS_PR_DAY, 2*(sizeof(double)), Double_Compare);
    
    for (i = 0; i < HOURS_PR_DAY; i++) {
        printf("[DEBUG] The sorted prices are hour [%2.0f]: %.2f\n", prices[i][1], prices[i][0]);
    }

    return prices;
}

/* A comparing function for qsort, typecasting input to doubles so it will be sorted properly */
int Double_Compare (const void *x, const void *y) {
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) return -1;
  if (xx > yy) return  1;
  return 0;
}

/* A function that fetches 24 lines worth of prices from file fp, then assigns them to numbered 2d array */
void Load_Prices (char* str, FILE *fp, double prices[][2]) {
    int i;
    for (i = 0; i < HOURS_PR_DAY; i++) {
        fgets(str, 13, fp);
        printf("[DEBUG] The price fetched is: %s", str);
        prices[i][1] = i;
        prices[i][0] = strtod(str, str+6);
    }
    printf("\n\n");
    return;
}

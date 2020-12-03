#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "calculate_prices.h"

/** Opens a price data file and load the prices into the given 2D array.
 *  @param prices[i/o] 2D array of doubles containing the prices for each hour in a day (1-24).
 *  @param sort[i] Boolean to determine whether the price array should be sorted or not (true = 1, false = 0).
 *  @param day[i] The day prices should be loaded from (1-365).
 *  @return Returns -1 if the price data could not be loaded, and 1 otherwise. */
int Calculate_Prices (double prices[][2], int sort, int day) {
    FILE *fp = fopen(PRICES_FILE, "r");

    if (fp == NULL) {
        printf("Error opening file '%s'.\n", PRICES_FILE);
        return -1;
    }
;
    Load_Prices (fp, prices, day);

    if (sort)
        qsort(prices, HOURS_PER_DAY, 2*(sizeof(double)), Double_Compare);
    return 1;
}

/** Reads a line in the input data file based on the input day, and assign the hourly prices to the price array.
 *  @param fp[i] File with price data.
 *  @param prices[i/o] 2D array of doubles containing the prices for each hour in a day (1-24).
 *  @param day[i] The day prices should be loaded from (1-365). */
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

/** Compare function for qsort, used in Calculate_Prices().
 *  Prioritizes the lowest price for each hour. */
int Double_Compare (const void *x, const void *y) {
  double xx = *(double*)x, yy = *(double*)y;
  if (xx < yy) 
        return -1;
  if (xx > yy) 
        return  1;
  return 0;
}

/** Prints a list of the input price array.
 *  @param prices[i] 2D array of doubles containing the prices for each hour in a day (1-24). */
void List_Prices (double prices[][2]) {
    int i;
    for(i = 0; i < 24; i++)
        printf("[%.2d - %.2d] is priced at %.2f.\n", 
                (int) prices[i][1], ((int) prices[i][1] + 1), prices[i][0]);
}

/** Sets the current day based on user input and recalculates the prices.
 *  @param prices[i/o]  2D array of doubles containing the prices for each hour in a day (1-24). 
 *  @param price_day[i/o]   The day that prices are loaded from. Can be edited for debugging. */
void Change_Day (double prices[][2], int *price_day) {
    char temp_input[MAX_INPUT];

    printf("Please enter the day you wish to load prices from: ");
    fgets (temp_input, MAX_INPUT, stdin);

    if (sscanf(temp_input, "%d", price_day) == 1) {
        if (*price_day > 365) {
            printf("Day is too high. Changed day to %d.\n", *price_day % 365);
            *price_day = *price_day % 365;
        }
        else if (*price_day <= 0) {
            *price_day = 1;
            printf("Day can't be 0 or below. Changed day to 1.\n");
        }
    }
    else
        printf("Please use numbers only.\n");

    Calculate_Prices (prices, 0, *price_day);
    printf("Prices have been initialized from day: %d.\n", *price_day);
}

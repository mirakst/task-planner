#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "calculate_prices.h"

/** Opens a price data file and load the prices into the given 2D array.
 *  @param prices[i/o] 2D array of doubles containing the prices for each hour in a day (1-24).
 *  @param sort[i] Boolean to determine whether the price array should be sorted or not (true = 1, false = 0).
 *  @param day[i] The day prices should be loaded from (1-365).
 *  @return Returns -1 if the price data could not be loaded, and 1 otherwise. */
int Calculate_kWh_Data (double data[][2], int sort, int day, int use_emissions) {
    FILE *fp = (use_emissions) ? fopen(EMISSIONS_FILE, "r") : fopen(PRICES_FILE, "r");

    if (fp == NULL) {
        printf("Error opening file '%s'.\n", PRICES_FILE);
        return -1;
    }

    Load_kWh_Data (fp, data, day);
    fclose(fp);
    return 1;
}

/** Reads a line in the input data file based on the input day, and assign the hourly prices to the price array.
 *  @param fp[i] File with price data.
 *  @param prices[i/o] 2D array of doubles containing the prices for each hour in a day (1-24).
 *  @param day[i] The day prices should be loaded from (1-365). */
void Load_kWh_Data (FILE *fp, double data[][2], int day) {
    int i;
    double value;

    for (i = 1; i < day; i++)
        fscanf(fp, "%*[^\n] %*[^ ]");

    for (i = 0; i < HOURS_PER_DAY + 6; i++) {
        if (i != 24) 
            fscanf(fp, "%lf;", &value);
        else {
            fscanf(fp, "%*[^\n]");
            fscanf(fp, "%*[^ ]");
        }
        data[i][1] = i;
        data[i][0] = value;
    }
}

/** Prints a list of the input price array.
 *  @param prices[i] 2D array of doubles containing the prices for each hour in a day (1-24). */
void List_kWh_Data (double data[][2], int use_emissions) {
    int i;
    for(i = 0; i < 24; i++) {
        if (use_emissions)
            printf("[%.2d - %.2d] = %3d g/kWh\n", 
                    (int) data[i][1], (int) (data[i][1] + 1), (int) data[i][0]);
        else
            printf("[%.2d - %.2d] = %.2f DKK\n", 
                    (int) data[i][1], (int) (data[i][1] + 1), data[i][0]);
    }
}

/** Sets the current day based on user input and recalculates the prices.
 *  @param prices[i/o] 2D array of doubles containing the prices for each hour in a day (1-24). 
 *  @param current_day[i/o] The day that prices are loaded from. Can be edited for debugging. */
void Change_Day (double data[][2], int *current_day, int use_emissions) {
    char temp_input[MAX_INPUT];

    printf("Please enter the day you wish to load prices from: ");
    fgets (temp_input, MAX_INPUT, stdin);

    if (sscanf(temp_input, " %d", current_day) == 1) {
        if (*current_day > 365) {
            printf("Day is too high. Changed day to %d.\n", *current_day % 365);
            *current_day = *current_day % 365;
        }
        else if (*current_day <= 0) {
            *current_day = 1;
            printf("Day can't be 0 or below. Changed day to 1.\n");
        }
    }
    else
        printf("Please use numbers only.\n");

    Calculate_kWh_Data (data, 0, *current_day, use_emissions);
    printf("Prices have been initialized from day: %d.\n", *current_day);
}
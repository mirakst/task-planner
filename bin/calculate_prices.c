#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "calculate_prices.h"

/** Opens the relevant data file based on the use_emissions bool.
 *  Reads a line in the input data file based on the input day, and assign the hourly values to the data array.
 *  @param data[i/o] Array of doubles containing the price or emission for each hour in a day (1-24).
 *  @param day[i] The day data should be loaded from (1-365). 
 *  @param use_emissions[i] Bool that determines what data to read (0 = prices, 1 = emissions).
 *  @return -1 if the data could not be loaded, and 1 otherwise. */
int Load_kWh_Data (double data[], int current_day, int use_emissions) {
    FILE *fp = (use_emissions) ? fopen(EMISSIONS_FILE, "r") : fopen(PRICES_FILE, "r");
    int i = 1;
    double value;
    
    if (fp == NULL) {
            printf("Error opening file '%s'.\n", PRICES_FILE);
            return -1;
    }

    do {
        if (current_day > 1)
            fscanf(fp, "%*[^\n] ");
        fscanf(fp, "%*[^ ]");
        i++;
    } while (i < current_day);

    for (i = 0; i < HOURS_PER_DAY * 2; i++) {
        if (current_day >= 364 && i > 23)
                break;
        if (i != 24) 
            fscanf(fp, "%lf;", &value);
        else {
            fscanf(fp, "%*[^\n]");
            fscanf(fp, "%*[^ ]");
            fscanf(fp, "%lf;", &value);
        }
        data[i] = value;
    }

    fclose(fp);
    return 1;
}

/** Prints a list of the input price array.
 *  @param data[i/o] Array of doubles containing the price or emission for each hour in a day (1-24).
 *  @param use_emissions[i] Bool that determines what data to read (0 = prices, 1 = emissions). */
void List_kWh_Data (double data[], int use_emissions) {
    int i;
    for(i = 0; i < HOURS_PER_DAY; i++) {
        if (use_emissions)
            printf("[%.2d - %.2d] = %3d g/kWh\n", 
                    i, (i + 1) % 24, (int) data[i]);
        else
            printf("[%.2d - %.2d] = %.2f DKK\n", 
                    i, (i + 1) % 24, data[i]);
    }
}

/** For debugging. Sets the current day based on user input and recalculates data.
 *  @param data[i/o] Array of doubles containing the price or emission for each hour in a day (1-24).
 *  @param current_day[i/o] The day that prices are loaded from. 
 *  @param use_emissions[i] Bool that determines what data to read (0 = prices, 1 = emissions). */
void Change_Day (double data[], int *current_day, int use_emissions) {
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

    Load_kWh_Data (data, *current_day, use_emissions);
    printf("Prices have been initialized from day: %d.\n", *current_day);
}
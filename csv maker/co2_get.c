#include <stdio.h>

#define INTERVAL_PER_HOUR 12

void Print_Function (int, int, FILE*, FILE*);

int main (void) {
    char *filein = "co2emisDK.csv",
         *fileout = "co2_emission_kwh_19.csv";
    FILE *fin,
         *fout;
    int days_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        i,
        j;

    fin = fopen (filein, "r");
    fout = fopen (fileout, "w");
    
    for (i = 0; i < 12; i++)
        Print_Function (i, days_per_month[i], fin, fout);

    fclose(fin);
    fclose(fout);
    return 1;
}

void Print_Function (int month, int days_month, FILE *fin, FILE *fout) {
    int accum_emission = 0,
        emission = 0,
        hour,
        day,
        interval;
    for (day = 0; day < days_month; day++) {
        fprintf(fout, "%.2d.%.2d.2019: ", day + 1, month + 1);
        for (hour = 0; hour < 24; hour++) {
            for (interval = 0; interval < INTERVAL_PER_HOUR; interval++) {
                fscanf(fin, "%*[^,] %*c %*[^,] %*c %d %*[^\n]", &emission);

                accum_emission += emission; 
            }
            emission = (accum_emission / INTERVAL_PER_HOUR);
            accum_emission = 0;
            fprintf(fout, "%d;", emission);
        }
    fprintf(fout, "\n");
    }
}

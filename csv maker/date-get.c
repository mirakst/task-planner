#include <stdio.h>

void Read_And_Print (int, int, FILE*, FILE*);

int main (void) {
    char *filein = "prices.txt",
         *fileout = "prices_2019.csv";
    FILE *fin,
         *fout;
    int days_per_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        i,
        j;

    fin = fopen (filein, "r");
    fout = fopen (fileout, "w");
    
    for (i = 0; i < 12; i++)
        Read_And_Print (i, days_per_month[i], fin, fout);

    fclose(fin);
    fclose(fout);
    return 1;
}

void Read_And_Print (int month, int days_month, FILE *fin, FILE *fout) {
    double price;
    int hour, day;

    for (day = 0; day < days_month; day++) {
        fprintf(fout, "%.2d.%.2d.2019: ", day + 1, month + 1);

        for (hour = 0; hour < 24; hour++) {
            fscanf(fin, "%lf", &price);
            price = (1.2745+0.00409+(price/1000))*1.25;
            fprintf(fout, "%.2f;", price);
        }
    fprintf(fout, "\n");
    }
}

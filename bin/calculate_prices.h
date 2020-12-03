#define HOURS_PER_DAY 24
#define MAX_INPUT 20
#define PRICES_FILE "config/prices_2019.csv"

int Double_Compare (const void *, const void *);
void Load_Prices (FILE*, double[][2], int);
int Calculate_Prices (double[][2], int, int);
void List_Prices (double[][2]);
void Change_Day (double[][2], int *);

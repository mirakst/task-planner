#define HOURS_PR_DAY 24
#define PRICES_FILE "prices.txt"

int Double_Compare (const void *, const void *);
void Load_Prices (char*, FILE*, double[][2]);
void Calculate_Prices (double[][2]);

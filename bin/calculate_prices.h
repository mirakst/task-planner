#define HOURS_PER_DAY 24
#define PRICES_FILE "config/pricelist"

int Double_Compare (const void *, const void *);
void Load_Prices (char*, FILE*, double[][2]);
int Calculate_Prices (double[][2], int);
void List_Prices (double[][2]);
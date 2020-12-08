#define HOURS_PER_DAY 24
#define MAX_INPUT 20
#define PRICES_FILE "config/prices_2019.csv"
#define EMISSIONS_FILE "config/emissions_2019.csv"

int Double_Compare (const void *, const void *);
int Load_kWh_Data (double[], int, int);
int Calculate_kWh_Data (double[], int, int);
void List_kWh_Data (double[], int);
void Change_Day (double[], int *, int);

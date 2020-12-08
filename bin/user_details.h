#define FILE_USER_DETAILS "config/settings"
#define USERNAME_MAX 50
#define HOURS_PER_DAY 24
#define SETTINGS_MAX 30

/** The user struct.
 * Contains all user settings.
 * @param available_hours[] Array of ints that keep track of user availability for all hours of a day (1 = available, 0 = unavailable).
 * @param user_name String with the name of the user.
 * @param ignore_availability Boolean that determines whether the available hours should be ignored when running the Find_Start_Hour() function.
 * @param use_emissions Boolean that determines which data to use in Find_Start_Hour() (0 = prices, 1 = emissions). */
typedef struct User_Details {
    int available_hours[HOURS_PER_DAY];
    char user_name [USERNAME_MAX];
    int ignore_availability,
        use_emissions;
} User;

void Reset_User (User *user);
int Get_Bool_Input(void);
double Get_Double_Input(int);
void First_Time_Setup (User *);
void Get_Name (User *);
void Reset_Available_Hours (int[HOURS_PER_DAY]);
void Set_Available_Hours (User *);
void Print_User (User);
int Load_User_Details (User *);
int Save_User_Details (User);
void Set_Ignore_Availability (User *);
void Set_Data_Type(User *);
void Print_Settings (User *);
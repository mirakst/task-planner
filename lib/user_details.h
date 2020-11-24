#define FILE_USER_DETAILS "user_details.txt"

typedef struct User_Details {
    int available_schedule[24][1];
    char user_name [100];
}User;

void Get_Name (User *, int *);
void Reset_Available_Hours (int time[][1]);
void Get_Available_Hours (int time[][1], int *);
void Save_To_File (User, int, int);
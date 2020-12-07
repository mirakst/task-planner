#ifndef I_H
#define I_H

#define TASK_NAME_MAX 21

int Get_Bool_Input(void) {
    char temp_string[TASK_NAME_MAX],
        bool_input;
        
    fgets(temp_string, TASK_NAME_MAX, stdin);
    sscanf(temp_string, " %c", &bool_input);
    while (tolower(bool_input) != 'y' && tolower(bool_input) != 'n' && bool_input != '\n') {
            printf("Please only input y or n: ");
            fgets(temp_string, TASK_NAME_MAX, stdin);
            sscanf(temp_string, " %c", &bool_input);
        }

    return bool_input == 'y' ? 1 : 0;
}

double Get_Double_Input(int maxinput) {
    char temp_string[TASK_NAME_MAX];
    double res;
    
    fgets(temp_string, TASK_NAME_MAX, stdin);
    while (sscanf(temp_string, " %lf", &res) != 1 || (res > maxinput || res <= 0)) {
        printf("Please enter a number (max = %d): ", maxinput);
        fgets(temp_string, TASK_NAME_MAX, stdin);
    }
    return res;
}

#endif
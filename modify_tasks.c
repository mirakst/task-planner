/* Navne - Mails? - Gruppe - Software */
/* For plebs  <      > ellers anders røv {   } for the cool kids -=xXx<({[L 3 3 7]})>xXx=- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SENTINEL "exit"
#define TASK_NAME_MAX 20
#define MAX_TASKS 10
#define INPUT_MAX 50

struct task {
    char name[TASK_NAME_MAX];
    int power;
    int use_time;
};
typedef struct task task;

void string_to_lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
}

void Print_List(task *, int);
task Add_Task(void);
task Add_Task_DEBUG(char *, int, int);
task Remove_Task(void);
void Print_Help (void);

int main (void) {
    task task_list[MAX_TASKS];
    char cmd_input[INPUT_MAX];
    int task_amount = 0,
        task_id = 0;

    task_list[0] = Add_Task_DEBUG("Opvaskning", 55, 180);
    task_list[1] = Add_Task_DEBUG("Vaskemaskine", 45, 180);
    task_list[2] = Add_Task_DEBUG("Stoevsugning", 35, 30);
    task_list[3] = Add_Task_DEBUG("Haandmixing", 5, 10);
    task_list[4] = Add_Task_DEBUG("Mikroovning", 30, 5);
    task_amount = 5;

    printf("\n---------------- Modify tasks ----------------\n");

    do {
        /* Get user input and convert to lower case letters */
        printf("\nPlease enter a command: ");
        scanf(" %s", cmd_input);
        string_to_lower(cmd_input);

        /* If "add": Get info and add the task*/
        if(!strcmp(cmd_input, "add")) {
            if(task_amount == MAX_TASKS) {
                printf("The maximum amount of tasks has been reached.\n");
            }
            else {
                task_list[task_amount] = Add_Task();
                task_amount++; 
            }
        }

        if(!strcmp(cmd_input, "list"))
            Print_List(task_list, task_amount);
        
        if(!strcmp(cmd_input, "help"))
            Print_Help();

        if(!strcmp(cmd_input, "remove")) {
            
            printf("Enter task id to remove: ");

            if(scanf(" %d", &task_id) == 0) {
                printf("Wrong command fuckhead\n");
            } 
            else {
                task_list[(task_id - 1)] = Remove_Task();

                if(!strcmp(task_list[(task_id - 1)].name, "NULL"))
                    printf("Task %d was successfully removed.\n", task_id);
            }
        }
        
    } while (strcmp(cmd_input, SENTINEL) != 0);
    
    return EXIT_SUCCESS;
}

void Modify_Tasks () {
    
}

void Print_Help (void) {
    printf("\n------------------ Commands ------------------\n");
    printf("%-12s -- Lists all tasks.\n", "List");
    printf("%-12s -- Add a new task.\n", "Add");
    printf("%-12s -- Remove a task\n", "Remove");
    printf("%-12s -- Go back to main.\n", "Back");
    printf("----------------------------------------------\n");
    return;
}

task Add_Task(void) {
    task result;
    char name[TASK_NAME_MAX];
    int power = 0, use_time = 0;

    printf("\nTask name (max %d): ", TASK_NAME_MAX);
    scanf(" %s", &name);
    printf("Power usage (watts): ");
    scanf(" %d", &power);
    printf("Task duration (min): ");
    scanf(" %d", &use_time);

    strcpy(result.name, name);
    result.power = power;
    result.use_time = use_time;
    return result;
}

task Add_Task_DEBUG(char *name, int power, int use_time) {
    task result;
    strcpy(result.name, name);
    result.power = power;
    result.use_time = use_time;
    return result;
}

/* Få sorteret arrayet efter det er fjernet, for at få NULL til sidst */
task Remove_Task(void) {
    task result;
    strcpy(result.name, "NULL");
    result.power = 0;
    result.use_time = 0;
    return result;
}

/*void Write_To_File() {

}

void Read_File() {
    
}*/

void Print_List(task *task_list, int task_amount) {
    int i;
    printf("\n----------------- Task  list -----------------\n");  
    printf("%13s%22s%11s\n", "Name", "Power", "Use time");    
    for (i = 0; i < task_amount; i++) {
        if (strcmp(task_list[i].name, "NULL") != 0) {
            printf("Task %2d: %-20s %3d W %6d min\n", (i + 1), task_list[i].name, task_list[i].power, task_list[i].use_time);
        }
    }
    printf("----------------------------------------------\n");
}


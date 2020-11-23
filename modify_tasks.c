/* -- DEPRECATED -- Use tasks.c instead! */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SENTINEL "exit"
#define EMPTY_TASK_NAME "none"
#define TASK_NAME_MAX 20
#define TASK_AMOUNT_MAX 10
#define INPUT_MAX 50

struct task {
    char name[TASK_NAME_MAX];
    int power;
    int use_time;
};
typedef struct task task;

/* From main.c - converts string to lower-case */
void string_to_lower(char *str) {
    int i;
    for (i = 0; i < INPUT_MAX; i++)
        str[i] = tolower(str[i]);
}

/* Prototypes */
void Print_List(task *, int);
task Add_Task(void);
task Add_Task_DEBUG(char *, int, int);
task Remove_Task(void);
void Print_Help (void);
void Sort_Tasks (task *);
int Task_Compare (const void *, const void *);

int main (void) {
    task task_list[TASK_AMOUNT_MAX];
    char cmd_input[INPUT_MAX],
         temp_task_name[TASK_NAME_MAX];
    int task_amount = 0,
        task_id = 0;

    /* Initialize tasks to be empty */
    for (int i = 0; i < TASK_AMOUNT_MAX; i++) {
        task_list[i] = Remove_Task();
    }
    
    /* Adds 5 different tasks [FOR DEBUGGING] */
    task_list[0] = Add_Task_DEBUG("Opvaskning", 55, 180);
    task_list[1] = Add_Task_DEBUG("Vaskemaskining", 45, 180);
    task_list[2] = Add_Task_DEBUG("Stoevsugning", 35, 30);
    task_list[3] = Add_Task_DEBUG("zzzzzzzzzz", 5, 10);
    task_list[4] = Add_Task_DEBUG("Mikroovning", 30, 5);
    task_amount = 5;

    printf("\n---------------- Modify tasks ----------------\n");

    do {
        /* Get user input and convert to lower case letters */
        printf("\nPlease enter a command: ");
        scanf(" %s", cmd_input);
        string_to_lower(cmd_input);

        if(!strcmp(cmd_input, "add")) {
            if(task_amount == TASK_AMOUNT_MAX) {
                printf("The maximum amount of tasks has been reached.\n");
            }
            else {
                task_list[task_amount] = Add_Task();
                task_amount++; 
            }
        }

        if (!strcmp(cmd_input, "list"))
            Print_List(task_list, task_amount);
        
        if (!strcmp(cmd_input, "help"))
            Print_Help();

        if (!strcmp(cmd_input, "remove")) {
            if (scanf(" %d", &task_id) == 0 || task_id < 1 || task_id > task_amount) {
                printf("The id was not recognized.\n");
            } 
            else {
                strcpy(temp_task_name, task_list[task_id - 1].name);
                task_list[(task_id - 1)] = Remove_Task();
                if (!strcmp(task_list[(task_id - 1)].name, EMPTY_TASK_NAME))
                    printf("Task: %s was successfully removed.\n", temp_task_name);
                
                task_amount--;
                Sort_Tasks(task_list);
            }
        }


    } while (strcmp(cmd_input, SENTINEL) != 0);
    
    return EXIT_SUCCESS;
}

/* Prints all the available commands  */
void Print_Help (void) {
    printf("\n------------------ Commands ------------------\n");
    printf("%-11s -- Lists all tasks.\n", "List");
    printf("%-11s -- Add a new task.\n", "Add");
    printf("%-11s -- Remove a task\n", "Remove (id)");
    printf("%-11s -- Go back to main.\n", "Back");
    printf("----------------------------------------------\n");
}

/* Gets  input from the user and returns it as a task */
task Add_Task (void) {
    task result;
    char name[TASK_NAME_MAX];
    int power = 0, use_time = 0;

    printf("\nTask name (max %d): ", TASK_NAME_MAX);
    scanf(" %s", &name);
    printf("Power usage (watts): ");
    scanf(" %d", &power);
    printf("Task duration (min): ");
    scanf(" %d", &use_time);    

    if(!strcmp(name, EMPTY_TASK_NAME) || power <= 0 || use_time <= 0) {
        printf("Error in user input, cancelling...\n"); return Remove_Task();
    }
    printf("Task: %s was successfully added.\n", name);
    strcpy(result.name, name);
    result.power = power;
    result.use_time = use_time;
    return result;
}

/* Returns a task with the given input parameters [FOR DEBUGGING] */
task Add_Task_DEBUG (char *name, int power, int use_time) {
    task result;
    strcpy(result.name, name);
    result.power = power;
    result.use_time = use_time;
    return result;
}

/* Returns an 'empty' task */
task Remove_Task (void) {
    task result;
    strcpy(result.name, EMPTY_TASK_NAME);
    result.power = 0;
    result.use_time = 0;    
    return result;
}

/* Sorts the input array with qsort and the Task_Compare function */
void Sort_Tasks (task *task_list) {
    qsort(task_list, TASK_AMOUNT_MAX, sizeof(task), Task_Compare);
}

/* Compares two tasks, and lets a non-empty task come first */
int Task_Compare (const void *ip1, const void *ip2) {
    const task *task1 = (task *) ip1,
               *task2 = (task *) ip2;

    if(strcmp(task1->name, EMPTY_TASK_NAME) == 0 && strcmp(task2->name, EMPTY_TASK_NAME) != 0)
        return 1;
    else if(strcmp(task1->name, EMPTY_TASK_NAME) != 0 && strcmp(task2->name, EMPTY_TASK_NAME) == 0)
        return -1;
    else
        return 1;
}

/* Prints the list of all tasks, excluding the empty tasks */
void Print_List(task *task_list, int task_amount) {
    int i;
    if(task_amount == 0) {
        printf("There are currently no tasks. Enter 'add' to begin adding some.\n"); return;
    }
    printf("\n----------------- Task  list -----------------\n");  
    printf("%13s%22s%11s\n", "Name", "Power", "Use time");    
    for (i = 0; i < TASK_AMOUNT_MAX; i++) {
        if (strcmp(task_list[i].name, EMPTY_TASK_NAME) != 0) {
            printf("Task %2d: %-20s %3d W %6d min\n", (i + 1), task_list[i].name, task_list[i].power, task_list[i].use_time);
        }
    }
    printf("----------------------------------------------\n");
}

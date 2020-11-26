#define FILE_TASKLIST "config/tasklist.cfg"
#define TASK_AMOUNT_MAX 10
#define TASK_NAME_MAX 20
#define EMPTY_TASK_NAME "none"
#define MIN_PER_HOUR 60
#define W_PER_KW 1000
#define READ_LINE_MAX 70

struct task {
    int bool_is_passive;

    char name[TASK_NAME_MAX];
    int power,
        duration_min,
        start_hr;
    double kW,
           duration_hrs;
};
typedef struct task task;

void Initialize_Tasks(task *, int *);
int Load_Tasks (task *, int *);
double Calculate_kW (task);
double Calculate_Hours (task);
int Save_Tasks (task *, int);
void Print_Task_List(task *, int);
void Add_Task (task *, int *);
void Get_Task_Input (char *, char *, int *, int *, int *);
void Remove_Task (task *, int *, int);
void Sort_Task_List (task *);
int Compare_Tasks (const void *, const void *);
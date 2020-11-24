#define FILE_TASKLIST "tasklist.txt"
#define TASK_AMOUNT_MAX 10
#define TASK_NAME_MAX 20
#define EMPTY_TASK_NAME "none"

struct task {
    char name[TASK_NAME_MAX];
    int power;
    int duration;
    double kWh;
};
typedef struct task task;

void Initialize_Tasks(task *, int *);
int Load_Tasks (task *, int *);
int Save_Tasks (task *, int);
void Print_Task_List(task *, int);
void Add_Task (task *, int *);
void Remove_Task (task *, int *, int);
void Sort_Task_List (task *);
int Compare_Tasks (const void *, const void *);
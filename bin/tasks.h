#define FILE_CONTROL     "config/controltask.cfg"
#define FILE_TASKLIST    "config/tasklist.cfg"
#define EMPTY_TASK_NAME  "none"
#define TASK_AMOUNT_MAX    20 /* Funny number */
#define TASK_NAME_MAX      20
#define MIN_PER_HOUR       60
#define W_PER_KW         1000
#define READ_LINE_MAX     100

/* FARLIG GLOBAL VARIABEL 
 * hold jer væk */
static int current_weekday = 0;

struct task {
    /* Basic info */
    int bool_is_passive;
    char name[TASK_NAME_MAX];
    double power,
           kWh,
           duration;
    
    /* Assignment */
    int is_assigned,
        start_hr,
        end_hr,
        days[7];
    double min_price,
           max_price,
           acc_price;
};
typedef struct task task;

double Calculate_kWh (task);

void Reset_Task (task *);
int Load_Tasks (task *, int *, char *);
int Save_Tasks (task *, int, char *);

void Add_Task (task *, int *, struct tm);
void Remove_Task (task *, int *, int);
void Edit_Task (task *, int, int);

void Set_Task_Status (int *);
void Set_Task_Duration (double *);
void Set_Task_Power (double *);
void Set_Task_Name (char *);
void Set_Task_Days (int *);

void Sort_Task_List (task *);
int Compare_Tasks (const void *, const void *);
void Print_Task_List(task *, int, struct tm, int);

/* Testing */
void Test_Tasks(void);
int Test_Load_Tasks(task *);
int Test_Save_Tasks(task *);
int Test_Remove_Task(task *);
int Test_KW(task);
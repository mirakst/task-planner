#include <stdio.h>
#include "tasks.h"

int main (void) {
    task task_list[TASK_AMOUNT_MAX];   
    int task_amount  = 0, task_id = 0;
    int file_status= 0;                 
 
    Initialize_Tasks(task_list, &task_amount);

    file_status = Load_Tasks(task_list, &task_amount);                           
    if (file_status == 1)                                                        
        printf("Loaded %d tasks successfully.\n", task_amount);                   
    else if (file_status == -1)                                              
        printf("Failed to load task configuration file: %s.\n", FILE_TASKLIST);  
    
    Print_Task_List(task_list, task_amount);            

    Add_Task(task_list, &task_amount);             
    Print_Task_List(task_list, task_amount);            

   file_status = Save_Tasks(task_list, task_amount);                            
   if(file_status == 1)                                                         
        printf("Saved %d tasks successfully.\n", task_amount);                  
   else if (file_status == -1)                                                 
       printf("Failed to save task configuration file: %s.\n", FILE_TASKLIST);
 
    return 0;
}
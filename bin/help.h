#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"

#define TRUE 1
#define FALSE 0

/* Help functions called by commands */
void Print_Welcome (void);
void Help_All(void);
void Help_Tasks(int);
void Help_Data(int);
void Help_Settings(int);
void Help_Settings_Extended(void);
void Help_Tasks_Extended(void);
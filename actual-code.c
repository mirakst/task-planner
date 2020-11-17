#include <stdio.h>

int test_function (int);
void hallihallo2 (int diller);
void fuckdam(void);

int main (void) {
    int i = 1;
    int num = 69;

    printf("Now, this is pod racing!\n");
    printf("Danny D knows whats up\n");
    printf("Nikolaj er mongol,\n");
    printf("Nikolaj er en dreng på 12 år :)\n");
    printf("Nikolajs mor er 24 år gammel.\n");
    printf("Nikolaj er en fucking stor mongol!!\n");
    printf("Num multiplied by 2 is: %d\n", test_function(num));
    fuckdam();
    return 1;
}

int test_function (int num) {
    printf("The number is: %d\n", num);
    return num * 2;
}

void hallihallo (int diller) {
    printf("%d\n", diller);
    
    return;
}

void fuckdam(void){
    int i;

    char noice[] = {'N', 'i', 'k', 'o', 'l', 'a', 'j', ' ', 'e', 'r', ' ', 'd', 'u', 'm'};

    for (i = 0; i < 14; i++)
    {
        printf("%c", noice[i]);
    }
    
    printf("\n");
    printf("Fuck DamDumDude\n");
    
    return;
}
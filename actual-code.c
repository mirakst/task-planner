#include <stdio.h>
int test_function (int);

void fuckdam(void);
void hallihallo (void);

int main (void) {
    int num = 69;
    printf("Now, this is pod racing!\n");
    printf("Danny D knows whats up");
    printf("Nikolaj er mongol,\n");
    printf("Nikolaj er en dreng på 12 år :)");
    printf("Nikolajs mor er 24 år gammel.\n");
    printf("Nikolaj er en fucking stor mongol!!\n");
    num = test_function(num) * 2;
    printf("Num multiplied by 2 is: %d", num);
    fuckdam();
    return 1;
}

int test_function (int num) {
    printf("The number is: %d.\n", num);
    return num + 1;
}

void fuckdam(void){
    printf("Fuck DamDumDude\n");
    return;
}

void hallihallo (void) {

}
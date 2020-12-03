#include <stdio.h>
#include <string.h>
#include "graphics.h"

/** Prints a line. Wraps around a word if has_word = 1 
 * @param [i] int has_word whether a word is present on the line or not.
 * @param [i] string of the word that would be wrapped around. */
void Print_Line(int has_word, char *word) {
    int i,
        word_length = strlen(word),
        total_length = 0;
    double side_length = (LINE_LENGTH - word_length) / 2;

    if (!has_word) {
        for (i = 0; i <= LINE_LENGTH; i++) {
            printf("%c", LINE_CHAR);
        }
        printf("\n");
    }
    else {
        for (i = 0; i < side_length; i++) {
            printf("%c", LINE_CHAR);
            total_length++;
        }
        printf(" %s ", word);
            
        total_length += (word_length + 2);
        while (total_length <= LINE_LENGTH) {
            printf("%c", LINE_CHAR);
            total_length++;
        }
        printf("\n");
    }
}































































































































































/* wtf */
void Print_Square(void) {
    printf("\n\n\n");
    printf("%c%c%c%c%c%c%c%c%c%c\n", 201, 203, 203, 203, 203, 203, 203, 203, 203, 187);
    printf("%c%c%c%c%c%c%c%c%c%c\n", 204, 206, 206, 206, 206, 206, 206, 206, 206, 185);
    printf("%c%c%c%c%c%c%c%c%c%c\n", 204, 206, 206, 206, 206, 206, 206, 206, 206, 185);
    printf("%c%c%c%c%c%c%c%c%c%c\n", 204, 206, 206, 206, 206, 206, 206, 206, 206, 185);
    printf("%c%c%c%c%c%c%c%c%c%c\n", 200, 202, 202, 202, 202, 202, 202, 202, 202, 188);
    printf("\n\n\n");
}
#include <stdio.h>
#include <string.h>
#include "graphics.h"

/** Prints a line. Prints a centered word if has_word = 1 
 * @param has_word[i] Bool that determines whether to print a word on the line (0 = false, 1 = true).
 * @param word[i] String of the word to print on the line. */
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
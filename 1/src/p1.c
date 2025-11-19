#include <stdlib.h>
#include <stdio.h>
// Since conio.h is literally unavailable on linux the only solution to this would be to install ncurses.
// I COULD install the linux conio.h implementation but that would be utterly useless.
#include <curses.h>
int main() {
    int *array = NULL;
    unsigned length, i; 
    printf("Enter length of array: ");
    scanf("%d", &length);
    if (length > 0) {
        if ((array = (int*) malloc(length * sizeof(int))) != NULL) {
            for (i = 0; i < length; i++) {
                array[i] = i * i;
            }
        } else {
            printf("Error: can't allocate memory");
        }
    }
    if (array != NULL) {
        free(array);
    }
    getch();
    return 0;
}


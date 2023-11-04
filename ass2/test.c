#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void) {
    int difficulty, length;
    char *name[10];
    printf("Enter three characters\n");
    char command;
    int res = scanf(" %c", &command);
    if(res == EOF)
        return 0;
    else if(command == 'r') {
        scanf(" %s %d %d", &name, &difficulty, &length);
        printf("command is %c\n", command);
        printf("name is %s, and difficulty is %d, and length is %d\n", name, difficulty, length);
    }
    return 0;
}
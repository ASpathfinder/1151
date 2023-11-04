#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STR_LEN 20

struct route {
    // The name of the climbing route
    char name[MAX_STR_LEN];
    // The difficulty of the route
    int difficulty;
    // The length of the route in metres
    int length;
    // A pointer to the next `struct route` in the logbook 
    struct route *next;
};
 
struct route *create_route(
    char name[MAX_STR_LEN], 
    int difficulty, 
    int length
);

int main(void) {
    printf("Welcome to 1511 Climb! \n");
    printf("Log all of your climbing adventures here! \n");

    struct route *test_route = create_route("Eternity", 18, 22);
    // Print out all it's fields.
    printf("\nname: %s\n", test_route->name);
    printf("difficulty: %d\n", test_route->difficulty);
    printf("length: %d\n", test_route->length);
    if (test_route->next == NULL) {
        printf("next field: (nil)\n");
    }

    printf("\nGoodbye\n");

    return 0;
}

struct route *create_route(
    char name[MAX_STR_LEN],
    int difficulty, 
    int length
) {
    struct route *new_route;
    new_route = (struct route *)malloc(100*sizeof(int));
    strcpy(new_route->name, name);
    new_route->difficulty = difficulty;
    new_route->length = length;

    return new_route;
}

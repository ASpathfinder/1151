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

struct logbook {
    // A pointer to the first `struct route` in the list
    struct route *routes;
};

struct route *create_route(
    char name[MAX_STR_LEN], 
    int difficulty, 
    int length
);
void scan_string(char string[MAX_STR_LEN]);
int scan_token(char *buffer, int buffer_size);
void print_usage(void);
void print_one_route(int position, struct route *route);


int main(void) {
    printf("Welcome to 1511 Climb! \n");
    printf("Log all of your climbing adventures here! \n");
    
    struct logbook *current_logbook;
    current_logbook = (struct logbook *)malloc(100*sizeof(int));
    char command;
    
    printf("Enter command: ");
    while(scanf(" %c", &command) != EOF) {
        if(command == '?')
            print_usage();
        else if(command == 'r') {
            char name[MAX_STR_LEN];
            int difficulty, length;

            scan_string(name);
            scanf(" %d %d", &difficulty, &length);

            struct route *new_route = create_route(name, difficulty, length);
            struct route *head_route = current_logbook->routes;
            if(head_route != NULL) {
                struct route *current_route = head_route;
                while(current_route->next != NULL) {
                    current_route = current_route->next;
                }
                current_route->next = new_route;
            } else
                current_logbook->routes = new_route;
            
            printf("Route '%s' added successfully!\n", new_route->name);
        } else if(command == 'p') {
            struct route *current_route = current_logbook->routes;
            if(current_route == NULL)
                printf("There are no routes in this logbook!");
            else {
                int i = 0;
                while(current_route != NULL) {
                    print_one_route(++i, current_route);
                    current_route = current_route->next;
                }
            }
        }
        
        printf("Enter command: ");
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

void scan_string(char string[MAX_STR_LEN]) {
    scan_token(string, MAX_STR_LEN);
}

int scan_token(char *buffer, int buffer_size) {
    if (buffer_size == 0) {
        return 0;
    }

    char c;
    int i = 0;
    int num_scanned = 0;

    // Consume all leading whitespace
    scanf(" ");

    // Scan in characters until whitespace
    while (i < buffer_size - 1
        && (num_scanned = scanf("%c", &c)) == 1 
        && !isspace(c)) {
        buffer[i++] = c;
    }

    if (i > 0) {
        buffer[i] = '\0';
    }

    return num_scanned;
}

void print_usage(void) {
    printf(
        "=====================[     1511 Climb     ]=====================\n"
        "      ===============[     Usage Info     ]===============      \n"
        "  r [name] [difficulty] [length]                                \n"
        "    Append a climbing route to the end of the logbook           \n"
        "  p                                                             \n"
        "    Print out all the climbing routes in the logbook            \n"
        "                                                                \n"
        "  f [difficulty_min] [difficulty_max]                           \n"
        "    Filter routes by difficulty                                 \n"
        "  i [name] [difficulty] [length] [route_to_insert_before]       \n"
        "    Insert a climbing route into the logbook before             \n"
        "    `route_to_insert_before`                                    \n"
        "  l [route_1] [route_2] [length_change]                         \n"
        "    Change length of all routes between `route_1` and `route_2` \n"
        "  s [route_1] [route_2]                                         \n"
        "    Swap 2 routes                                               \n"
        "                                                                \n"
        "  a [climber] [attempt_type] [rating] [route]                   \n"
        "    Add an attempt of `route` by `climber`                      \n"
        "  P                                                             \n"
        "    Print out all the routes and attempts in the logbook        \n"
        "  R [route]                                                     \n"
        "    Remove a route and all associated attempts                  \n"
        "  D [climber]                                                   \n"
        "    Delete all attempts by `climber`                            \n"
        "  d [climber_1] [climber_2]                                     \n"
        "    Duplicate all of `climber_2`s attempts for `climber_1`      \n"
        "                                                                \n"
        "  c [route_1] [route_2]                                         \n"
        "    Combine 2 routes                                            \n"
        "  L [route]                                                     \n"
        "    Print the climber leaderboard for `route`                   \n"
        "                                                                \n"
        "  ?                                                             \n"
        "    Show help                                                   \n"
        "================================================================\n"
    );
}

void print_one_route(int position, struct route *route) {
    int line_length = MAX_STR_LEN + 2;
    printf("/--------- \\/ ---------\\\n");

    char position_str[MAX_STR_LEN];
    sprintf(position_str, "Route #%d", position);
    int padding = line_length - strlen(position_str);
    printf(
        "|%*s%s%*s|\n", padding / 2, "", position_str, (padding + 1) / 2, ""
    );

    padding = line_length - strlen(route->name);
    printf("|%*s%s%*s|\n", padding / 2, "", route->name, (padding + 1) / 2, "");

    printf("|    Difficulty: %2d    |\n", route->difficulty);
    printf("|    Length (m): %2d    |\n", route->length);
    printf("\\--------- || ---------/\n");
}
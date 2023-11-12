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
int command_r_validate_arguments(char *name, int difficulty, int length, struct logbook* logbook);
int command_l_validate_length_change(struct route* head, struct route* end, int length_changed);
struct route *search_for_route(char *name, struct logbook* logbook);
struct route *get_route_by_index(int index, struct route* head);
struct route *insert_route_before(char *name, int difficulty, int length, struct route *head, char *route_to_insert_before);
int route_index(char *name, struct route* head);

int main(void) {
    printf("Welcome to 1511 Climb! \n");
    printf("Log all of your climbing adventures here! \n");
    
    struct logbook *current_logbook;
    current_logbook = (struct logbook *)malloc(sizeof(struct logbook));
    char command;
    
    printf("Enter command: ");
    while(scanf(" %c", &command) != EOF) {
        // Stage 1.2
        if(command == '?')
            print_usage();
        // Stage 1.3
        else if(command == 'r') {
            char name[MAX_STR_LEN];
            int difficulty, length;

            scan_string(name);
            scanf(" %d %d", &difficulty, &length);
            if(command_r_validate_arguments(name, difficulty, length, current_logbook)) {
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
            }
        // Stage 1.4
        } else if(command == 'p') {
            struct route *current_route = current_logbook->routes;
            if(current_route == NULL)
                printf("There are no routes in this logbook!\n");
            else {
                int i = 0;
                while(current_route != NULL) {
                    print_one_route(++i, current_route);
                    current_route = current_route->next;
                }
            }
        // Stage 2.1
        } else if(command == 'f') {
            int difficulty_min, difficulty_max;
            scanf(" %d %d", &difficulty_min, &difficulty_max);
            struct route *current_route = current_logbook->routes;

            if((difficulty_min <= 0 || difficulty_min > 39 || difficulty_max <= 0 || difficulty_max > 39) || (difficulty_min > difficulty_max)) {
                printf("ERROR: Difficulty range invalid!\n");
                current_route = NULL;
            } else
                printf("Routes between difficulty %d and %d:\n", difficulty_min, difficulty_max);
            
            int i = 0;
            while(current_route != NULL) {
                i++;
                if(current_route->difficulty >= difficulty_min && current_route->difficulty <= difficulty_max) {
                    print_one_route(i, current_route);
                }
                current_route = current_route->next;
            }
        // stage 2.2
        } else if(command == 'i') {
            char name[MAX_STR_LEN];
            char route_to_insert_before[MAX_STR_LEN];
            int difficulty, length;

            scan_string(name);
            scanf(" %d %d", &difficulty, &length);
            scan_string(route_to_insert_before);
            if(command_r_validate_arguments(name, difficulty, length, current_logbook)) {
                if(search_for_route(route_to_insert_before, current_logbook))
                    current_logbook->routes = insert_route_before(name, difficulty, length, current_logbook->routes, route_to_insert_before);
                else
                    printf("ERROR: No route with the name '%s' exists in this logbook\n", route_to_insert_before);
            }
        // stage 2.3
        } else if(command == 'l') {
            char route_1_name[MAX_STR_LEN], route_2_name[MAX_STR_LEN];
            struct route *route_1;
            struct route *route_2;
            int route_1_index, route_2_index;
            int length_change;

            scan_string(route_1_name);
            scan_string(route_2_name);
            scanf(" %d", &length_change);
            
            route_1_index = route_index(route_1_name, current_logbook->routes);
            route_2_index = route_index(route_2_name, current_logbook->routes);
            if(route_1_index == -1) {
                printf("ERROR: No route with the name '%s' exists in this logbook\n", route_1_name);
            } else if(route_2_index == -1) {
                printf("ERROR: No route with the name '%s' exists in this logbook\n", route_2_name);
            } else {
                if(route_1_index > route_2_index) {
                    route_1 = get_route_by_index(route_2_index, current_logbook->routes);
                    route_2 = get_route_by_index(route_1_index, current_logbook->routes);
                } else {
                    route_1 = get_route_by_index(route_1_index, current_logbook->routes);
                    route_2 = get_route_by_index(route_2_index, current_logbook->routes);
                }
                if(command_l_validate_length_change(route_1, route_2, length_change)) {
                    struct route *current_route = route_1;
                    struct route *prev_route = NULL;
                    int old_length;
                    while(prev_route != route_2) {
                        old_length = current_route->length;
                        current_route->length = current_route->length + length_change;
                        printf("Length of '%s' updated from %dm to %dm\n", current_route->name, old_length, current_route->length);
                        prev_route = current_route;
                        current_route = current_route->next;
                    }
                }
            }
        // stage 2.4
        } else if(command == 's') {
            char route_1_name[MAX_STR_LEN], route_2_name[MAX_STR_LEN];
            struct route *route_1;
            struct route *route_2;
            int route_1_index, route_2_index;

            scan_string(route_1_name);
            scan_string(route_2_name);
            
            route_1_index = route_index(route_1_name, current_logbook->routes);
            route_2_index = route_index(route_2_name, current_logbook->routes);
            if(route_1_index == -1) {
                printf("ERROR: No route with the name '%s' exists in this logbook\n", route_1_name);
            } else if(route_2_index == -1) {
                printf("ERROR: No route with the name '%s' exists in this logbook\n", route_2_name);
            } else {
                if(route_1_index > route_2_index) {
                    route_1 = get_route_by_index(route_2_index, current_logbook->routes);
                    route_2 = get_route_by_index(route_1_index, current_logbook->routes);
                } else {
                    route_1 = get_route_by_index(route_1_index, current_logbook->routes);
                    route_2 = get_route_by_index(route_2_index, current_logbook->routes);
                }
                if(route_1 == route_2){
                    printf("ERROR: Cannot swap '%s' with itself\n", route_1->name);
                } else {
                    route_1_index = route_index(route_1->name, current_logbook->routes);
                    route_2_index = route_index(route_2->name, current_logbook->routes);
                    if(route_1_index == 0) {
                        if(route_2_index - route_1_index == 1) {
                            route_1->next = route_2->next;
                            route_2->next = route_1;
                        } else {
                            int route_2_prev_index = route_2_index-1;
                            struct route *route_2_prev = get_route_by_index(route_2_prev_index, current_logbook->routes);
                            struct route *route_2_next = route_2->next;
                            route_2->next = route_1->next; 
                            route_1->next = route_2_next;
                            route_2_prev->next = route_1;
                        }
                        current_logbook->routes = route_2; // reset route head
                    } else {
                        if(route_2_index - route_1_index == 1) {
                            struct route *route_1_prev = get_route_by_index(route_1_index-1, current_logbook->routes);
                            route_1->next = route_2->next;
                            route_2->next = route_1;
                            route_1_prev->next = route_2;
                        } else {
                            int route_1_prev_index = route_1_index-1;
                            int route_2_prev_index = route_2_index-1;
                            struct route *route_1_prev = get_route_by_index(route_1_prev_index, current_logbook->routes);
                            struct route *route_2_prev = get_route_by_index(route_2_prev_index, current_logbook->routes);
                            struct route *route_2_next = route_2->next;
                            route_2->next = route_1->next;
                            route_1_prev->next = route_2;
                            route_1->next = route_2_next;
                            route_2_prev->next = route_1;
                        }
                    }
                    printf("'%s' swapped positions with '%s'!\n", route_1_name, route_2_name);
                }
            }
        }
        
        printf("Enter command: ");
    }
    
    printf("\nGoodbye\n");

    return 0;
}

// Stage 1.1
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

// Stage 1.5
int command_r_validate_arguments(char *name, int difficulty, int length, struct logbook* logbook) {
    if(difficulty <= 0 || difficulty > 39) {
        printf("ERROR: Route difficulty must be between 1 and 39\n");
        return 0;
    }
    if(length <= 0 || length > 60) {
        printf("ERROR: Route length must be between 1m and 60m\n");
        return 0;
    }
    if(search_for_route(name, logbook)) {
        printf("ERROR: A route with the name '%s' already exists in this logbook\n", name);
        return 0;
    }
    return 1;
}

int command_l_validate_length_change(struct route* head, struct route* end, int length_changed) {
    struct route *current_route = head;
    struct route *prev_route = NULL;
    int new_length;
    while(prev_route != end) {
        new_length = current_route->length + length_changed;
        if(new_length <= 0 || new_length > 60) {
            printf("ERROR: Invalid length change! No route lengths have been changed\n");
            return 0;
        }
        prev_route = current_route;
        current_route = current_route->next;
    }
    return 1;
}

struct route *search_for_route(char *name, struct logbook* logbook) {
    struct route *current_route = logbook->routes;
    while(current_route != NULL) {
        if(!strcmp(name, current_route->name))
            return current_route;
        current_route = current_route->next;
    }
    return 0;
}

int route_index(char *name, struct route* head) {
    struct route *current_route = head;
    int i=0;
    while(current_route != NULL) {
        if(!strcmp(name, current_route->name))
            return i;
        i++;
        current_route = current_route->next;
    }
    return -1;
}

struct route *get_route_by_index(int index, struct route* head) {
    struct route *current_route = head;
    int i = 0;
    while(current_route != NULL) {
        if(i++ == index)
            return current_route;
        current_route = current_route->next;
    }
    return NULL;
}

struct route *insert_route_before(char *name, int difficulty, int length, struct route *head, char *route_to_insert_before) {
    struct route *new_one = create_route(name, difficulty, length);
    
    int n = route_index(route_to_insert_before, head);

    if(n == 0) {
        new_one->next = head;
        printf("Route '%s' inserted successfully!\n", new_one->name);
        return new_one;
    }

    struct route *current = head;
    int number = 1;
    while(current != NULL) {
        if(number++ == n) {
            struct route *old_next = current->next;
            current->next = new_one;
            new_one->next = old_next;
            printf("Route '%s' inserted successfully!\n", new_one->name);
            return head;
        }
        if(current->next == NULL)
            break;
        current = current->next;
    }

    if(current == NULL)
        printf("Route '%s' inserted successfully!\n", new_one->name);
        return new_one;

    current->next = new_one;
    
    printf("Route '%s' inserted successfully!\n", new_one->name);
    return head;
}


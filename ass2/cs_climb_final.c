// Program name
//
// This program was written by [your name] (z5555555)
// on [date]
//
// assignment 2


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////////////
///////////////////////////      Constants       ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided constants
#define MAX_STR_LEN 20
#define HELP_COMMAND '?'

// Provided enums
// This enum only has to be used in Stage 3.1 onwards
enum attempt_type {FIRST_GO, SUCCESS, FAIL, INVALID_TYPE};

////////////////////////////////////////////////////////////////////////////////
//////////////////////////  USER DEFINED TYPES  ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided structs
struct attempt {
    char climber[MAX_STR_LEN];
    enum attempt_type type;
    int rating;
    struct attempt *next;
};

// Represents a single climbing route in the logbook
struct route {
    // The name of the climbing route
    char name[MAX_STR_LEN];
    // The difficulty of the route
    int difficulty;
    // The length of the route in metres
    int length;
    // A pointer to the next `struct route` in the logbook 
    struct route *next;
    // Stores a pointer to the head of the `attempts` list
    struct attempt *attempts;
};

// Represents the logbook that contains info on each climbing route
struct logbook {
    // A pointer to the first `struct route` in the list
    struct route *routes;
};
 
////////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void print_usage(void); 
void print_one_route(int position, struct route *route);
void scan_string(char string[MAX_STR_LEN]);
enum attempt_type scan_attempt_type();
void print_one_attempt(
    char climber[MAX_STR_LEN], 
    enum attempt_type type, 
    int rating
);

// Additional provided function prototypes
// You won't need to use these functions!
// We use them just to implement some of the provided helper functions.
int scan_token(char *buffer, int buffer_size);
enum attempt_type string_to_type(char *type_str);
void type_to_string(char *buf, enum attempt_type type);

////////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct logbook *create_logbook();
struct route *create_route(
    char name[MAX_STR_LEN], 
    int difficulty, 
    int length
);
int command_r_validate_arguments(char *name, int difficulty, int length, struct logbook* logbook);
int command_l_validate_length_change(struct route* head, struct route* end, int length_changed);
struct route *search_for_route(char *name, struct logbook* logbook);
struct route *get_route_by_index(int index, struct route* head);
struct route *insert_route_before(char *name, int difficulty, int length, struct route *head, char *route_to_insert_before);
int route_index(char *name, struct route* head);
struct attempt *create_attempt(char *climber, enum attempt_type type, int rating);
struct attempt *search_for_attempt(char *climber, struct attempt *head);
int get_climbers_attempt_insert_point(char *climber, struct attempt *head);
struct attempt *insert_climber_latest_attempt(char *climber, enum attempt_type type, int rating, struct route *route);
float average_attempt(struct attempt *head);
struct route *remove_route(char *name, struct route **head);
void free_route_memory(struct route *route);
void free_attempt_memory(struct attempt *attempt);
void free_logbook_memory(struct logbook *logbook);
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    printf("Welcome to 1511 Climb! \n");
    printf("Log all of your climbing adventures here! \n");
    
    struct logbook *current_logbook = create_logbook();
    char command;
    
    printf("Enter command: ");
    while(scanf(" %c", &command) != EOF) {
        // Stage 1.2
        if(command == HELP_COMMAND)
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
        // stage 3.1
        } else if(command == 'a') {
            char climber[MAX_STR_LEN], route_name[MAX_STR_LEN];
            int rating;
            scan_string(climber);
            enum attempt_type type = scan_attempt_type();
            scanf(" %d", &rating);
            scan_string(route_name);
            if(type == INVALID_TYPE)
                printf("ERROR: Attempt type invalid\n");
            else if(rating < 0 || rating > 3)
                printf("ERROR: Rating must be between 0 and 3\n");
            else {
                struct route *current_route = search_for_route(route_name, current_logbook);
                if(!current_route)
                    printf("ERROR: No route with the name '%s' exists in this logbook\n", route_name);
                else if((current_route->attempts = insert_climber_latest_attempt(climber, type, rating, current_route)) != NULL){
                    printf("Logged attempt of '%s' by %s\n", current_route->name, climber);
                }
            }
        // stage 3.2
        } else if(command == 'P') {
            struct route *current_route = current_logbook->routes;
            if(current_route == NULL)
                printf("There are no routes in this logbook!\n");
            else {
                int i = 0;
                while(current_route != NULL) {
                    printf("Route #%d: %s\nDifficulty: %d | Length: %dm | Avg rating: %.1lf/3.0\n",
                    ++i,
                    current_route->name,
                    current_route->difficulty,
                    current_route->length,
                    average_attempt(current_route->attempts));
                    struct attempt *current_attempt = current_route->attempts;
                    while(current_attempt != NULL) {
                        print_one_attempt(current_attempt->climber, current_attempt->type, current_attempt->rating);
                        current_attempt = current_attempt->next;
                    }
                    current_route = current_route->next;
                }
            }
        } else if(command == 'R') {
            char route_name[MAX_STR_LEN];
            scan_string(route_name);
            struct route *removed_route = remove_route(route_name, &(current_logbook->routes));
            if(removed_route) {
                free_route_memory(removed_route);
                printf("Removed route '%s' from logbook\n", route_name);
            } else
                printf("ERROR: No route with the name '%s' exists in this logbook\n", route_name);
        }
        
        printf("Enter command: ");
    }
    
    free_logbook_memory(current_logbook);
    printf("\nGoodbye\n");

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////  YOUR FUNCTIONS //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Allocate memory for a new struct logbook and return a pointer to it
//
// Parameters:
//      None
// 
// Returns:
//      new_logbook - a pointer to the newly created struct logbook
struct logbook *create_logbook() {
    struct logbook *new_logbook = malloc(sizeof(struct logbook));
    new_logbook->routes = NULL;
    return new_logbook;
}


// STAGE 1.1
// create a route with given arguments
//
// Parameters:
//      name        - name of the route
//      difficulty  - difficulty of the route
//      length      - length of the route
// 
// Returns:
//      new_route   - a pointer to the newly created struct route
struct route *create_route(
    char name[MAX_STR_LEN],
    int difficulty, 
    int length
) {
    struct route *new_route;
    new_route = malloc(sizeof(struct route));
    strcpy(new_route->name, name);
    new_route->difficulty = difficulty;
    new_route->length = length;
    new_route->attempts = NULL;
    new_route->next = NULL;

    return new_route;
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
    return NULL;
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

    if(current == NULL) {
        printf("Route '%s' inserted successfully!\n", new_one->name);
        return new_one;
    }

    current->next = new_one;
    
    printf("Route '%s' inserted successfully!\n", new_one->name);
    return head;
}

struct attempt *create_attempt(char *climber, enum attempt_type type, int rating) {
    struct attempt *new_attempt = malloc(sizeof(struct attempt));
    strcpy(new_attempt->climber, climber);
    new_attempt->type = type;
    new_attempt->rating = rating;
    new_attempt->next = NULL;
    return new_attempt;
}

struct attempt *search_for_attempt(char *climber, struct attempt *head) {
    struct attempt *current_attempt = head;
    while(current_attempt != NULL) {
        if(!strcmp(current_attempt->climber, climber)) {
            return current_attempt;
        }
        current_attempt = current_attempt->next;
    }
    return NULL;
}

int get_climbers_attempt_insert_point(char *climber, struct attempt *head) {
    struct attempt *current_attempt = head;
    int i = 0;
    int cmp_result = 0;
    while(current_attempt != NULL) {
        cmp_result = strcmp(current_attempt->climber, climber);
        if(cmp_result >= 0) {
            return i;
        } else {
            struct attempt *prev_attempt = current_attempt;
            current_attempt = current_attempt->next;
            i++;
            while(current_attempt != NULL && strcmp(prev_attempt->climber, current_attempt->climber) == 0) {
                i++;
                current_attempt = current_attempt->next;
            }
            if(current_attempt == NULL) {
                return -1;
            } else {
                continue;
            }
        }
    }
    return 0;
}

struct attempt *insert_climber_latest_attempt(char *climber, enum attempt_type type, int rating, struct route *route) {
    struct attempt *head = route->attempts;
    
    if(search_for_attempt(climber, head) && type == FIRST_GO) {
        printf("ERROR: '%s' has already attempted '%s' - they can't get it on their first go!\n", climber, route->name);
        return NULL;
    }

    int n = get_climbers_attempt_insert_point(climber, head);
    struct attempt *new_one = create_attempt(climber, type, rating);

    if(n == 0) {
        new_one->next = head;
        return new_one;
    }

    struct attempt *current = head;
    int number = 1;
    while(current != NULL) {
        if(number++ == n) {
            struct attempt *old_next = current->next;
            current->next = new_one;
            new_one->next = old_next;
            return head;
        }
        if(current->next == NULL)
            break;
        current = current->next;
    }

    if(current == NULL)
        return new_one;

    current->next = new_one;
    
    return head;
}

float average_attempt(struct attempt *head) {
    if(head == NULL)
        return 0.0;

    struct attempt *current_attempt = head;
    int sum = 0;
    int i = 0;
    while(current_attempt != NULL) {
        sum += current_attempt->rating;
        ++i;
        current_attempt = current_attempt->next;
    }
    return (float)sum / i;
}

struct route *remove_route(char *name, struct route **head) {
    struct route *current = *head;
    struct route *prev = NULL;
    while(current != NULL) {
        if(strcmp(current->name, name) == 0) {
            if(prev == NULL)
                *head = current->next;
            else
                prev->next = current->next;
            current->next = NULL;
            break;
        }
        prev = current;
        current = current->next;    
    }

    if(current == NULL)
        return NULL;

    return current;
}

void free_logbook_memory(struct logbook *logbook) {
    struct route *current_route = logbook->routes;
    struct route *next = NULL;
    while(current_route != NULL) {
        next = current_route->next;
        free_route_memory(current_route);
        current_route = next;
    }
    free(logbook);
}

void free_route_memory(struct route *route) {
    struct attempt *current_attempt = route->attempts;
    struct attempt *next = NULL;
    while(current_attempt != NULL) {
        next = current_attempt->next;
        free_attempt_memory(current_attempt);
        current_attempt = next;
    }
    free(route);
}

void free_attempt_memory(struct attempt *attempt) {
    free(attempt);
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////  PROVIDED FUNCTIONS  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Prints the 1511 Climb usage instructions,
// displaying the different commands and their arguments.
//
// Parameters:
//      None
// 
// Returns: 
//      None
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

// Prints a single route, displaying its position in the logbook, name, grade 
// and length. 
//
// Parameters:
//      position    - The position of the route in the logbook (the first 
//                        route will be route_number 1, second will be 2, etc)
//      route       - A pointer to the struct route to be printed
//
// Returns:   
//      None
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

// Scans a string into the provided char array, placing a '\0' at the end
// 
// Parameters:
//      string      - a char array of length MAX_STR_LEN to store the string 
//                    that is scanned in
// 
// Returns: 
//      None
// 
// Usage: 
// ```
//      char string[MAX_STR_LEN];
//      scan_string(string);
// ```
void scan_string(char string[MAX_STR_LEN]) {
    scan_token(string, MAX_STR_LEN);
}

// Scans a string and converts it to an enum attempt_type.
//
// Parameters: 
//      None
// 
// Returns:
//      The corresponding attempt_type, if the string was valid,
//      Otherwise, returns INVALID_TYPE.
// 
// Usage: 
// ```
//      enum attempt_type type = scan_attempt_type();
// ```
enum attempt_type scan_attempt_type(void) {
    char type[MAX_STR_LEN];
    scan_token(type, MAX_STR_LEN);
    return string_to_type(type);
}

// Prints a single attempt, displaying the climber name, attempt type and rating
//
// Parameters:
//      climber     - The name of the climber who attempted to climb a route
//      type        - The attempt type that was made (should be either 
//                    FIRST_GO, SUCCESS, or FAIL)
//      rating      - The star rating that the climber gave the route
// 
// Returns: 
//      None
void print_one_attempt(
    char climber[MAX_STR_LEN], 
    enum attempt_type type, 
    int rating
) {
    char type_string[MAX_STR_LEN];
    type_to_string(type_string, type);
    printf("        --> %s\n", type_string);
    printf("            Climber: %s\n", climber);
    printf("            Rating:  ");
    for (int i = 0; i < rating; i++) {
        printf("*");
    }
    printf("\n\n");
}

////////////////////////////////////////////////////////////////////////////////
////////////////////// ADDITIONAL PROVIDED FUNCTIONS ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// You don't need to use any of these, or understand how they work!
// We use them to implement some of the provided helper functions.

// Scans a single word into `buffer`, ignoring whitespace
// 
// Parameters: 
//      buffer      - the string to store the scanned word
//      buffer_size - the maximum size of string that can be scanned (including 
//                    the null terminator)
// Returns: 
//      The number of chars scanned successfully
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

// Converts a string to the corresponding enum attempt_type
//
// Parameters: 
//      type_str    - The string to be converted
// 
// Returns: 
//      The matching enum attempt_type, or `INVALID_TYPE` if `type_str` matches 
//      no valid attempt type
enum attempt_type string_to_type(char *type_str) {
    int len = MAX_STR_LEN;

    if (strncasecmp(type_str, "first_go", len) == 0) {
        return FIRST_GO;
    }
    if (strncasecmp(type_str, "success", len) == 0) {
        return SUCCESS;
    } 
    if (strncasecmp(type_str, "fail", len) == 0) {
        return FAIL;
    } 

    return INVALID_TYPE;
}

// Converts an enum attempt_type to a string
// 
// Parameters:
//      buf         - The string to store the attempt type
//      type        - The enum attempt_type to be converted
// 
// Returns:
//      None
void type_to_string(char *buf, enum attempt_type type) {
    if (type == FIRST_GO) {
        strcpy(buf, "FIRST GO");
    } else if (type == SUCCESS) {
        strcpy(buf, "SUCCESS");
    } else if (type == FAIL) {
        strcpy(buf, "FAIL");
    } else {
        strcpy(buf, "INVALID");
    }
}

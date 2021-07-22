#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>
#include <limits.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define PRETTY_PRINT 1
#define __DEBUG__ 1

#define SEED 13371337
#define MULTIPLE_OF_PROBLEM_SIZE 8
#define ERROR_MAX_MSG_SIZE 256
#define DEV_URANDOM_SIZE 256
#define DEV_URANDOM_FLAGS 0

//https://dev.to/tenry/terminal-colors-in-c-c-3dgc
//https://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#if PRETTY_PRINT == 1
    #define PRINTF_RED(x, ...)      printf(ANSI_COLOR_RED     x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_GREEN(x, ...)    printf(ANSI_COLOR_GREEN   x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_YELLOW(x, ...)   printf(ANSI_COLOR_YELLOW  x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_BLUE(x, ...)     printf(ANSI_COLOR_BLUE    x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_MAGENTA(x, ...)  printf(ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_CYAN(x, ...)     printf(ANSI_COLOR_CYAN    x ANSI_COLOR_RESET, __VA_ARGS__)

    #define PRINT_RED(x)      printf(ANSI_COLOR_RED     x ANSI_COLOR_RESET)
    #define PRINT_GREEN(x)    printf(ANSI_COLOR_GREEN   x ANSI_COLOR_RESET)
    #define PRINT_YELLOW(x)   printf(ANSI_COLOR_YELLOW  x ANSI_COLOR_RESET)
    #define PRINT_BLUE(x)     printf(ANSI_COLOR_BLUE    x ANSI_COLOR_RESET)
    #define PRINT_MAGENTA(x)  printf(ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET)
    #define PRINT_CYAN(x)     printf(ANSI_COLOR_CYAN    x ANSI_COLOR_RESET)
#else
    #define PRINTF_RED(x, ...)      printf(x, __VA_ARGS__)
    #define PRINTF_GREEN(x, ...)    printf(x, __VA_ARGS__)
    #define PRINTF_YELLOW(x, ...)   printf(x, __VA_ARGS__)
    #define PRINTF_BLUE(x, ...)     printf(x, __VA_ARGS__)
    #define PRINTF_MAGENTA(x, ...)  printf(x, __VA_ARGS__)
    #define PRINTF_CYAN(x, ...)     printf(x, __VA_ARGS__)

    #define PRINT_RED(x)      printf(x)
    #define PRINT_GREEN(x)    printf(x)
    #define PRINT_YELLOW(x)   printf(x)
    #define PRINT_BLUE(x)     printf(x)
    #define PRINT_MAGENTA(x)  printf(x)
    #define PRINT_CYAN(x)     printf(x)
#endif

//https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#define DEBUG_ARENA(...)     do { if (__DEBUG__) {__VA_ARGS__} } while(0)
#define DEBUG_PRINTF(x, ...) DEBUG_ARENA(PRINTF_YELLOW("%s:%d:%s():\t" x ANSI_COLOR_RESET, __FILE__, __LINE__, __func__, __VA_ARGS__);)
#define DEBUG_PRINT(x)       DEBUG_ARENA(PRINT_YELLOW(ANSI_COLOR_YELLOW "%s:%d:%s():\t" x ANSI_COLOR_RESET, __FILE__, __LINE__, __func__);)

enum Error_t {
    NONE = 0,
    MEMORY_ALLOCATION_FAILED
};

enum Vector_t {
    VECTOR_OF_ONES = 0,
    VECTOR_OF_ZEROS,
    VECTOR_OF_ALTERNATING_ONES_ZEROS,
    VECTOR_OF_RANDOMS,
    VECTOR_OF_TRUE_RANDOMS
};

typedef struct err_struct err_s;
typedef struct solv_iteration_data solver_iteration_data;
typedef struct iter_data_node iteration_data_node;
typedef struct iter_data_sll iteration_data_sll;

uint8_t *create_solution_table(uint8_t *(*p_creator)(size_t, err_s *), size_t p_size, err_s *);
uint8_t *create_vector_of_ones(size_t, err_s *);
uint8_t *create_vector_of_zeros(size_t, err_s *);
uint8_t *create_vector_of_alternating_ones_zeros(size_t, err_s *);
uint8_t *create_vector_of_randoms(size_t, err_s *);
// Please note that using this function will not get you reproducible results. Therefore, detailed logging should be
//  enabled while debugging, since it's not guaranteed the same behaviour will occur with the same program parameters.
uint8_t *create_vector_of_true_randoms(size_t, err_s *);
//returns the next 8bit random
uint8_t get_next_binary_rand();
//returns the next 8bit random
uint8_t get_next_binary_true_rand();
//problem size must be a multiple of 8
int size_is_multiple_of_problem_size(size_t problem_size);
char *get_error_type_string(int err_type);

//data handling
iteration_data_sll *initialize_iteration_data_list(err_s *);
iteration_data_node *create_iteration_data_sll_node(solver_iteration_data *, err_s *);
solver_iteration_data *create_iteration_data(const double, const uint64_t, const size_t, uint8_t *, err_s *);
void add_iteration_data_node_to_sll(iteration_data_sll *, solver_iteration_data *, err_s *ppErr); \
void free_iteration_data_sll(iteration_data_sll **);
void free_iteration_data_node(iteration_data_node **);
void free_iteration_data(solver_iteration_data **);

//https://stackoverflow.com/questions/2525310/how-to-define-and-work-with-an-array-of-bits-in-c
void set_bit(uint8_t *arr, int k);          //set   (one)
void clear_bit(uint8_t *arr, int k);        //clear (zero)
int test_bit(const uint8_t *arr, int k);    //test  (1 if 1, else 0)

struct err_struct {
    char *err_msg;
    enum Error_t err_type;
};

struct solv_iteration_data {
    double fitness;
    size_t size;
    uint64_t score;
    uint8_t *input_table;
};

struct iter_data_node {
    struct iter_data_node *next;
    solver_iteration_data *data; // maybe convert to void *? //if this is void * all funs need to be converted to void
};

struct iter_data_sll {
    iteration_data_node *head;
    size_t size;
};

uint64_t MAX_ITERATIONS;

#pragma clang diagnostic push
int main(int argc, char *argv[]) {
    err_s *err = malloc(sizeof(err_s));
    err->err_type = NONE;
    err->err_msg = NULL;

    srand(SEED); // calls to lrand48(); will now work correctly

    int creator_type = VECTOR_OF_ZEROS;
    size_t problem_size = 512;

    for (int i = 1; i < argc; ++i) {
        DEBUG_PRINTF("Got arg %d %s\n", i, argv[i]);
        //do nothing currently
        if (strcmp(argv[i], "--hello") == 0)        { printf("Hello World!\n");                 continue; }
        if (strcmp(argv[i], "--zeros") == 0)        { creator_type = VECTOR_OF_ZEROS;                  continue; }
        if (strcmp(argv[i], "--ones") == 0)         { creator_type = VECTOR_OF_ONES;                   continue; }
        if (strcmp(argv[i], "--alternating") == 0)  { creator_type = VECTOR_OF_ALTERNATING_ONES_ZEROS; continue; }
        if (strcmp(argv[i], "--random") == 0)       { creator_type = VECTOR_OF_TRUE_RANDOMS;           continue; }
        if (strcmp(argv[i], "--fixed_random") == 0) { creator_type = VECTOR_OF_RANDOMS;                continue; }
        if (strcmp(argv[i], "--size") == 0) {
            ++i; char *next;
            problem_size = strtol(argv[i], &next, (int) strlen(argv[i]));
            continue;
        }
    }

    MAX_ITERATIONS = problem_size * problem_size;
    if (MAX_ITERATIONS < problem_size) {
        DEBUG_PRINTF("MAX_ITERATIONS overflowed with current value %lu and problem_size %zu, setting to %llu\n",
                     MAX_ITERATIONS, problem_size, ULLONG_MAX);
        MAX_ITERATIONS = ULLONG_MAX;
    }

    if (!size_is_multiple_of_problem_size(problem_size)) {
        DEBUG_PRINTF("Problem size %zu not a multiple of %d, cannot continue\n",
                     problem_size, MULTIPLE_OF_PROBLEM_SIZE);
        exit(EXIT_FAILURE);
    }

    uint8_t * (*creator)(size_t, err_s *);

    #pragma clang diagnostic push
    #pragma ide diagnostic ignored "UnreachableCode"
    switch (creator_type) {
        case VECTOR_OF_ZEROS:
            creator = create_vector_of_zeros;
            break;
        case VECTOR_OF_ONES:
            creator = create_vector_of_ones;
            break;
        case VECTOR_OF_ALTERNATING_ONES_ZEROS:
            creator = create_vector_of_alternating_ones_zeros;
            break;
        case VECTOR_OF_RANDOMS:
            creator = create_vector_of_randoms;
            break;
        case VECTOR_OF_TRUE_RANDOMS:
            creator = create_vector_of_true_randoms;
            break;
        default:
            DEBUG_PRINTF("Invalid vector type initializator switch val %d\n", creator_type);
            exit(EXIT_FAILURE);
    }
    #pragma clang diagnostic pop
    uint8_t *solution_table = create_solution_table(creator, problem_size, err);

    DEBUG_ARENA(
            for (size_t i = 0; i < problem_size; ++i) {
                printf("%d", solution_table[i]);
                if ((i+1) % MULTIPLE_OF_PROBLEM_SIZE == 0) printf("\n");
            }
            printf("\n");
            );

    if (!solution_table) {
        DEBUG_PRINTF("%s", err->err_msg);
        exit(EXIT_FAILURE);
    }

    PRINT_BLUE("Starting iterations!\n");

    // call a compute() function with all the necessary parameters. it should:
    //  do iterations until convergence, or until MAX_ITERATIONS
    //  report the statistics on each iteration (write to file? perhaps also show to stdout with colours (!))
    //  report the final statistics
    //  all table data should be reported if __DEBUG__ is enabled
    // table data should be reported as such:
    //  length of array (problem size)
    //  sum of all elements
    //  sum of each 1/4
    //  sum of each 1/8
    //  sum of primary and secondary (for each 8th, 1/8+2/8+...+8/8) and the reverse (8/8+7/8+...+1/8) diagonal (?)
    //  avg of 1s in a row
    //  avg of 0s in a row
    //  mean of each of these two above
    //  entire table should be reported if __DEBUG__ is enabled
    // algorithm data that should be reported:
    //  tries, and success/score on each try
    //  all the above mentioned (rounded to the 4th decimal)
    //  score and fitness for each try
    //  detailed movements and detailed reporting of what code path was followed according to what happened

    return 0;
}
#pragma clang diagnostic pop

uint8_t *create_solution_table(uint8_t * (*p_creator)(size_t, err_s *), size_t p_size, err_s *ppErr) {
    uint8_t *sol_v = p_creator(p_size, ppErr);
    if (ppErr->err_type != NONE) {
        DEBUG_PRINTF("Got %s\n", ppErr->err_msg);
        exit(EXIT_FAILURE);
    }
    return sol_v;
}

uint8_t *create_vector_of_ones(size_t p_size, err_s *ppErr) {
    uint8_t *sol = malloc(p_size * sizeof (uint8_t));
    if (!sol) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        return NULL;
    }

    for (size_t i = 0; i < p_size; ++i) {
        sol[i] = 1;
    }

    return sol;
}

uint8_t *create_vector_of_zeros(size_t p_size, err_s *ppErr){
    uint8_t *sol = calloc(p_size, sizeof (uint8_t));
    if (!sol) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s at line %d file %s", errt, __LINE__, __FILE__);
        return NULL;
    }

    return sol;
}

uint8_t *create_vector_of_alternating_ones_zeros(size_t p_size, err_s *ppErr){
    uint8_t *sol = malloc(p_size * sizeof (uint8_t));
    if (!sol) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s at line %d file %s", errt, __LINE__, __FILE__);
        return NULL;
    }

    for (size_t i = 1; i <= p_size; ++i) {
        sol[i-1] = i%2 == 0;
    }

    return sol;
}

uint8_t *create_vector_of_randoms(size_t p_size, err_s *ppErr){
    uint8_t *sol = malloc(p_size * sizeof (uint8_t));
    if (!sol) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        return NULL;
    }

    for (size_t i = 0; i < p_size; ++i) {
        sol[i] = get_next_binary_rand();
    }
}

uint8_t *create_vector_of_true_randoms(size_t p_size, err_s *ppErr) {
    uint8_t *sol = malloc(p_size * sizeof (uint8_t));
    if (!sol) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        return NULL;
    }

    // https://man7.org/linux/man-pages/man2/getrandom.2.html
    for (size_t i = 0; i < p_size; ++i) {
        sol[i] = get_next_binary_true_rand();
    }
}

//returns the next binary random
uint8_t get_next_binary_rand(){
    return rand()%2 == 0;
}

//returns the next binary random TODO test if this actually works as intended
uint8_t get_next_binary_true_rand(){
    static uint8_t *dev_urandom_buff = NULL;
    static size_t   bytes_read;
    // initialization of static var
    if (!dev_urandom_buff) {
        dev_urandom_buff = malloc(DEV_URANDOM_SIZE * sizeof (uint8_t));
        bytes_read = 0;
    }

    if (bytes_read >= DEV_URANDOM_SIZE - 1 || bytes_read == 0) {
        bytes_read = 0;
        getrandom(dev_urandom_buff, DEV_URANDOM_SIZE, DEV_URANDOM_FLAGS);
    }

    return dev_urandom_buff[++bytes_read] % 2 == 0;
}

iteration_data_sll *initialize_iteration_data_list(err_s *ppErr) {
    iteration_data_sll *iter_sll = malloc(sizeof(iteration_data_sll));
    if (!iter_sll) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        return NULL;
    }
    iter_sll->head=NULL;
    iter_sll->size=0;

    return iter_sll;
}

iteration_data_node *create_iteration_data_sll_node(solver_iteration_data *iter_data, err_s *ppErr) {
    iteration_data_node *iter_data_node = malloc(sizeof(iteration_data_node));
    if (!iter_data_node) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        return NULL;
    }

    iter_data_node->data = iter_data;
    iter_data_node->next = NULL;

    return iter_data_node;
}

solver_iteration_data *create_iteration_data(const double fitness, const uint64_t score, const size_t p_size, uint8_t *input_table, err_s *ppErr) {
    solver_iteration_data *data = malloc(sizeof(solver_iteration_data));
    if (!data) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        return NULL;
    }
    data->size = p_size;
    data->fitness = fitness;
    data->score = score;
    data->input_table = input_table;

    return data;
}

void add_iteration_data_node_to_sll(iteration_data_sll *sll, solver_iteration_data *iter_data, err_s *ppErr) {
    // if head is initialized...
    if (sll->size != 0) {
        iteration_data_node *iter_node = sll->head;
        for (size_t i = 0; i < sll->size - 1; ++i) {
            iter_node = iter_node->next;
        }
        iter_node->next = create_iteration_data_sll_node(iter_data, ppErr);
    } else {
        sll->head = create_iteration_data_sll_node(iter_data, ppErr);
    }

    //TODO add debug check on ppErr

    if (ppErr->err_type != NONE) {
        return;
    }

    sll->size++;
}

void free_iteration_data_sll(iteration_data_sll **pp_iteration_data_sll) {
    iteration_data_node *iterator = (*pp_iteration_data_sll)->head;
    iteration_data_node *next_item = NULL;
    size_t s = (*pp_iteration_data_sll)->size;
    for (size_t i = 0; i < s; ++i) {
        next_item = iterator->next;
        free(iterator);
        iterator = next_item;
    }

    //TODO add debug prints

}

void free_iteration_data_node(iteration_data_node **pp_iter_data_node) {
    iteration_data_node *p_iter = *pp_iter_data_node;
    free_iteration_data(&(p_iter->data));
    p_iter = NULL;
}

void free_iteration_data(solver_iteration_data **pp_solver_iteration_data) {
    solver_iteration_data *p_solver = *pp_solver_iteration_data;
    free(p_solver->input_table);
    free(p_solver);
    p_solver = NULL;
}

void set_bit(uint8_t *arr, int k) {
    arr[k/8] |= 1 << (k%8);  // Set the bit at the k-th position in A[i]
}

void clear_bit(uint8_t *arr, int k) {
    arr[k/8] &= ~(1 << (k%8));
}

int test_bit(const uint8_t *arr, int k) {
    return ((arr[k/8] & (1 << (k%8) )) != 0 ) ;
}

int size_is_multiple_of_problem_size(size_t problem_size) {
    return problem_size % MULTIPLE_OF_PROBLEM_SIZE == 0;
}

char *get_error_type_string(int err_type) {
    switch (err_type) {
        case NONE:
            return "None";
        case MEMORY_ALLOCATION_FAILED:
            return "MEMORY_ALLOCATION_FAILED";

        default:
            return "UNKNOWN ERROR TYPE";
    }
}

#pragma clang diagnostic pop

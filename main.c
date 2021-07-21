#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#define __DEBUG__ 1
#define SEED 133713371337
#define MULTIPLE_OF_PROBLEM_SIZE 8

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

typedef struct err_struct {
    char *err_msg;
    unsigned char err_type;
} err_s;

typedef struct solv_iteration_data {
    double fitness;
    size_t size;
    uint64_t score;
    uint8_t input_table[];
} solver_iteration_data;

uint8_t *create_solution_table(uint8_t *(*p_creator)(size_t, err_s *), size_t p_size, err_s *);
uint8_t *create_vector_of_ones(size_t, err_s *);
uint8_t *create_vector_of_zeros(size_t, err_s *);
uint8_t *create_vector_of_alternating_ones_zeros(size_t, err_s *);
uint8_t *create_vector_of_randoms(size_t, err_s *);
//returns the next 8bit random
uint8_t get_next_8bit_rand();
//problem size must be a multiple of 8
int size_is_multiple_of_problem_size(size_t problem_size);
char *get_error_type_string(int err_type);

//https://stackoverflow.com/questions/2525310/how-to-define-and-work-with-an-array-of-bits-in-c
void set_bit(uint8_t *arr, int k);          //set   (one)
void clear_bit(uint8_t *arr, int k);        //clear (zero)
int test_bit(const uint8_t *arr, int k);    //test  (1 if 1, else 0)

int main(int argc, char *argv[]) {
    err_s *err = NULL;

    srand48(SEED); // calls to lrand48(); will now work correctly

    int creator_type = VECTOR_OF_ZEROS;
    size_t problem_size = 1024;

    for (int i = 1; i < argc; ++i) {
        if (__DEBUG__) fprintf(stderr, "Got arg %d %s\n", i, argv[i]);
        //do nothing currently
        if (strcmp(argv[i], "--hello") == 0)        printf("Hello World!\n");
        if (strcmp(argv[i], "--zeros") == 0)        creator_type = VECTOR_OF_ZEROS;
        if (strcmp(argv[i], "--ones") == 0)         creator_type = VECTOR_OF_ONES;
        if (strcmp(argv[i], "--alternating") == 0)  creator_type = VECTOR_OF_ALTERNATING_ONES_ZEROS;
        if (strcmp(argv[i], "--random") == 0)       creator_type = VECTOR_OF_TRUE_RANDOMS;
        if (strcmp(argv[i], "--fixed_random") == 0) creator_type = VECTOR_OF_RANDOMS;
        if (strcmp(argv[i], "--size") == 0) {
            ++i; char *next;
            problem_size = strtol(argv[i], &next, (int) strlen(argv[i]));
        }
    }
    fflush(stderr);

    if (!size_is_multiple_of_problem_size(problem_size)) {
        fprintf(stderr, "Problem size %zu not a multiple of %d, cannot continue\n",
                problem_size, MULTIPLE_OF_PROBLEM_SIZE);
        exit(EXIT_FAILURE);
    }

    uint8_t * (*creator)(size_t, err_s *);

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
        default:
            fprintf(stderr, "Invalid vector type initializator switch val %d\n", creator_type);
            exit(EXIT_FAILURE);
    }
    uint8_t *solution_table = create_solution_table(creator, problem_size, err);
    // TODO dynamic array of static arrays to hold solv_iteration_data mmap each array block to a getpagesize() from unistd.h

    // call a compute() function with all the necessary parameters. it should:
    //  do iterations until convergence
    //  report the statistics on each iteration (write to file? perhaps also show to stdout with colours (!))
    //  report the final statistics
    //  all table data should be reported if __DEBUG__ is enabled
    return 0;
}

uint8_t *create_solution_table(uint8_t * (*p_creator)(size_t, err_s *), size_t p_size, err_s *ppErr) {
    uint8_t *sol_v = p_creator(p_size, ppErr);
    if (ppErr->err_type != NONE) {
        fprintf(stderr, "Got error type %s\tline: %d\tfile: %s\n",
                get_error_type_string(ppErr->err_type), __LINE__, __FILE__);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    return sol_v;
}

uint8_t *create_vector_of_ones(size_t p_size, err_s *ppErr){
    //TODO implement
}

uint8_t *create_vector_of_zeros(size_t p_size, err_s *ppErr){
    //TODO implement
}

uint8_t *create_vector_of_alternating_ones_zeros(size_t p_size, err_s *ppErr){
    //TODO implement
}

uint8_t *create_vector_of_randoms(size_t p_size, err_s *ppErr){
    //TODO implement
}

//returns the next 8bit random
uint8_t get_next_8bit_rand(){
    //TODO implement
}

void set_bit(uint8_t *arr, int k) {
    arr[k/32] |= 1 << (k%32);  // Set the bit at the k-th position in A[i]
}

void clear_bit(uint8_t *arr, int k) {
    arr[k/32] &= ~(1 << (k%32));
}

int test_bit(const uint8_t *arr, int k) {
    return ((arr[k/32] & (1 << (k%32) )) != 0 ) ;
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

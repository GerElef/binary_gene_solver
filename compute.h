#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include "macros.h"
#include "globals.h"

#ifndef BINARY_GENE_SOLVER_DATA_STRUCTS_H
#define BINARY_GENE_SOLVER_DATA_STRUCTS_H

uint64_t MAX_ITERATIONS;

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

typedef struct err_struct Err_s;
typedef struct solv_iteration_data Solver_iteration_data;
typedef struct iter_data_node Iteration_data_node;
typedef struct iter_data_sll Iteration_data_sll;

uint8_t *(*getCreator(enum Vector_t creator_type))(size_t, Err_s *);

uint8_t *create_solution_table(uint8_t *(*p_creator)(size_t, Err_s *), size_t p_size, Err_s *);
uint8_t *create_vector_of_ones(size_t, Err_s *);
uint8_t *create_vector_of_zeros(size_t, Err_s *);
uint8_t *create_vector_of_alternating_ones_zeros(size_t, Err_s *);
uint8_t *create_vector_of_randoms(size_t, Err_s *);
// Please note that using this function will not get you reproducible results. Therefore, detailed logging should be
//  enabled while debugging, since it's not guaranteed the same behaviour will occur with the same program parameters.
uint8_t *create_vector_of_true_randoms(size_t, Err_s *);
//returns the next 8bit random
uint8_t get_next_binary_rand();
//returns the next 8bit random
uint8_t get_next_binary_true_rand();
//problem size must be a multiple of 8
int size_is_multiple_of_problem_size(size_t);
char *get_error_type_string(int err_type);
//returns 1 if it set error info otherwise 0
int set_error_info(Err_s *, int, char *, enum Error_t);
int check_error_info(Err_s *);
//data handling
Iteration_data_sll *initialize_iteration_data_list(Err_s *);
Iteration_data_node *create_iteration_data_sll_node(Solver_iteration_data *, Err_s *);
Solver_iteration_data *create_iteration_data(const double, const uint64_t, const size_t, uint8_t *, Err_s *);
void add_iteration_data_node_to_sll(Iteration_data_sll *, Solver_iteration_data *, Err_s *);
void free_iteration_data_sll(Iteration_data_sll **);
void free_iteration_data_node(Iteration_data_node **);
void free_iteration_data(Solver_iteration_data **);

//https://stackoverflow.com/questions/2525310/how-to-define-and-work-with-an-array-of-bits-in-c
void set_bit(uint8_t *arr, int k);          //set   (one)
void clear_bit(uint8_t *arr, int k);        //clear (zero)
int test_bit(const uint8_t *arr, int k);    //test  (1 if 1, else 0)

void *c_malloc(size_t b, const int line, const char *file, const char *func);
void *c_calloc(size_t n, size_t n_size, const int line, const char *file, const char *func);
void *c_realloc(void *p, size_t new_size, const int line, const char *file, const char *func);
void c_free(void *p, const int line, const char *file, const char *func);

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
    Solver_iteration_data *data; // maybe convert to void *? //if this is void * all funs need to be converted to void
};

struct iter_data_sll {
    Iteration_data_node *head;
    size_t size;
};

#endif //BINARY_GENE_SOLVER_DATA_STRUCTS_H

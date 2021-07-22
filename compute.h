#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>
#include <limits.h>

#ifndef BINARY_GENE_SOLVER_DATA_STRUCTS_H
#define BINARY_GENE_SOLVER_DATA_STRUCTS_H

extern uint64_t MAX_ITERATIONS;

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
//TODO check the SLL functions all work as intended
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

#endif //BINARY_GENE_SOLVER_DATA_STRUCTS_H

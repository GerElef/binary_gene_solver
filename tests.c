#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/random.h>
#include <limits.h>
#include "macros.h"
#include "compute.h"
#include <assert.h>

// TODO check everything works correctly
//  meaning, create tests!

#define TEST_MACROS                 1
#define TEST_SLL_AND_BOILERPLATE    0

#if __DEBUG__ == 1
    #define TEST_DEBUG_MACROS
#endif

#if PRETTY_PRINT == 1
    #define TEST_PRETTY_PRINT_MACROS
#endif

#if CUSTOM_MALLOC == 1
    #define TEST_CUSTOM_MALLOC_MACROS
#endif

#define TEST_DEBUG_ARENA_MACRO
#define TEST_DEBUG_PRINTF_MACRO
#define TEST_FOR_DEFER_MACRO
#define TEST_DEFER_GCC_LLVM_MACRO

#define TEST_TABLE_CREATION
#define TEST_RANDOM_GETTERS
#define TEST_SIZE_AND_ERRTYPE
#define TEST_SINGLE_LINKED_LIST
#define TEST_COMPUTE

void test_create_solution_table(size_t p_size);
void test_create_vector_of_ones(size_t);
void test_create_vector_of_zeros(size_t);
void test_create_vector_of_alternating_ones_zeros(size_t);
void test_create_vector_of_randoms(size_t);
void test_create_vector_of_true_randoms(size_t);
void test_get_next_binary_rand();
void test_get_next_binary_true_rand();
void test_size_is_multiple_of_problem_size(size_t problem_size);
void test_get_error_type_string();

void test_initialize_iteration_data_list();
void test_create_iteration_data_sll_node(solver_iteration_data *, err_s *);
void test_create_iteration_data(const double, const uint64_t, const size_t, uint8_t *, err_s *);
void test_add_iteration_data_node_to_sll(iteration_data_sll *, solver_iteration_data *, err_s *ppErr);
void test_free_iteration_data_sll(iteration_data_sll **);
void test_free_iteration_data_node(iteration_data_node **);
void test_free_iteration_data(solver_iteration_data **);


int main(int argc, char *argv[]) {
    err_s *err;

    srand(SEED); // calls to lrand48(); will now work correctly
    size_t problem_size = 512;
    MAX_ITERATIONS = problem_size * problem_size;

    uint8_t * (*creator)(size_t, err_s *) = getCreator(VECTOR_OF_ZEROS);

#if TEST_MACROS == 1

    #ifdef TEST_DEBUG_MACROS
        //ALREADY COVERED BY OTHER TESTS.
        // AT THE TIME OF WRITING 25/8/2021 THIS WAS COVERED BY TEST_DEBUG_ARENA_MACRO
    #endif

    #ifdef TEST_PRETTY_PRINT_MACROS
        //TODO
    #endif

    #ifdef TEST_CUSTOM_MALLOC_MACROS
        //TODO THE MOST IMPORTANT!!!!!!!!!!!!!!!
    #endif

    #ifdef TEST_DEBUG_ARENA_MACRO
        //TODO
    #endif

    #ifdef TEST_DEBUG_PRINTF_MACRO
        //TODO
    #endif

    #ifdef TEST_FOR_DEFER_MACRO
        //TODO
    #endif

    #ifdef TEST_DEFER_GCC_LLVM_MACRO
    //TODO
    #endif
    PRINT_MAGENTA("Macro tests passed!\n");

#endif

#if TEST_SLL_AND_BOILERPLATE == 1
    err = malloc(sizeof(err_s));
    err->err_type = NONE;
    err->err_msg = NULL;

    #ifdef TEST_TABLE_CREATION
        test_create_solution_table(problem_size);
        test_create_vector_of_ones(problem_size);
        test_create_vector_of_zeros(problem_size);
        test_create_vector_of_alternating_ones_zeros(problem_size);
        test_create_vector_of_randoms(problem_size);
        test_create_vector_of_true_randoms(problem_size);
    #endif

    #ifdef TEST_RANDOM_GETTERS
        test_get_next_binary_rand();
        test_get_next_binary_true_rand();
    #endif

    #ifdef TEST_SIZE_AND_ERRTYPE
        test_size_is_multiple_of_problem_size(problem_size);
        test_get_error_type_string();
    #endif

    #ifdef TEST_SINGLE_LINKED_LIST
        test_initialize_iteration_data_list();
        //TODO
        // test_create_iteration_data_sll_node(solver_iteration_data *, err_s *);
        // test_create_iteration_data(const double, const uint64_t, const size_t, uint8_t *, err_s *);
        // test_add_iteration_data_node_to_sll(iteration_data_sll *, solver_iteration_data *, err_s *ppErr);
        // test_free_iteration_data_sll(iteration_data_sll **);
        // test_free_iteration_data_node(iteration_data_node **);
        // test_free_iteration_data(solver_iteration_data **);
    #endif

    PRINT_MAGENTA("Sll and boilerplate tests passed!\n");

#endif

#ifdef TEST_COMPUTE
    //TODO

    PRINT_MAGENTA("COMPUTE tests passed!\n");
#endif

    PRINT_BLUE("All tests passed!\n");

    return EXIT_SUCCESS;
}

void test_create_solution_table(size_t p_size) {

}

void test_create_vector_of_ones(size_t p_size) {

}

void test_create_vector_of_zeros(size_t p_size) {

}

void test_create_vector_of_alternating_ones_zeros(size_t p_size) {

}

void test_create_vector_of_randoms(size_t p_size) {

}

void test_create_vector_of_true_randoms(size_t p_size) {

}

void test_get_next_binary_rand() {

}

void test_get_next_binary_true_rand() {

}

void test_size_is_multiple_of_problem_size(size_t problem_size) {

}

void test_get_error_type_string() {

}


void test_initialize_iteration_data_list() {

}

void test_create_iteration_data_sll_node(solver_iteration_data * sid, err_s *perr) {

}

void test_create_iteration_data(const double f, const uint64_t sc, const size_t s, uint8_t *it, err_s *perr) {

}

void test_add_iteration_data_node_to_sll(iteration_data_sll *idsll, solver_iteration_data *sid, err_s *pErr) {

}

void test_free_iteration_data_sll(iteration_data_sll **pidsll) {

}

void test_free_iteration_data_node(iteration_data_node **pidsll) {

}

void test_free_iteration_data(solver_iteration_data **psid) {

}

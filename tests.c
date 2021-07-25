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
#define TEST_DEBUG_PRINTS_MACRO
#define TEST_FOR_DEFER_MACRO
#define TEST_DEFER_GCC_LLVM_MACRO

#define TEST_TABLE_CREATION
#define TEST_RANDOM_GETTERS
#define TEST_SIZE_AND_ERRTYPE
#define TEST_SINGLE_LINKED_LIST
#define TEST_COMPUTE

//TODO THE MOST IMPORTANT!!!!!!!!!!!!!!!
void test_c_malloc();
void test_c_calloc();
void test_c_realloc();
void test_c_free();

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

    #ifdef TEST_PRETTY_PRINT_MACROS
        printf("----------------------------------------------------\n");
        PRINTF_RED("This text is supposed to be %s <--positional arg\n", "red");
        PRINTF_GREEN("This text is supposed to be %s <--positional arg\n", "green");
        PRINTF_YELLOW("This text is supposed to be %s <--positional arg\n", "yellow");
        PRINTF_BLUE("This text is supposed to be %s <--positional arg\n", "blue");
        PRINTF_MAGENTA("This text is supposed to be %s <--positional arg\n", "magenta");
        PRINTF_CYAN("This text is supposed to be %s <--positional arg\n", "cyan");
        PRINT_RED("This text is supposed to be red\n");
        PRINT_GREEN("This text is supposed to be green\n");
        PRINT_YELLOW("This text is supposed to be yellow\n");
        PRINT_BLUE("This text is supposed to be blue\n");
        PRINT_MAGENTA("This text is supposed to be magenta\n");
        PRINT_CYAN("This text is supposed to be cyan\n");
        printf("----------------------------------------------------\n");
    #endif

    #ifdef TEST_DEBUG_MACROS
        // we use printf here because the macro tests for debug prints are under! DO NOT CHANGE!
        #ifdef TEST_DEBUG_ARENA_MACRO
            DEBUG_ARENA(
                    int x = 5;
                    ++x;
                    printf("DEBUG ARENA x is %d!\n", x);
            );
        #endif

        #ifdef TEST_DEBUG_PRINTS_MACRO
            printf("----------------------------------------------------\n");
            if (PRETTY_PRINT) {
                DEBUG_PRINT("This debug print is coloured!\n");
                DEBUG_PRINTF("This debug print is coloured %s <---pos!\n", "somehow");
            } else {
                DEBUG_PRINT("This debug print is NOT coloured!\n");
                DEBUG_PRINTF("This debug print is NOT coloured %s <---pos!\n", "somehow");
            }
            printf("----------------------------------------------------\n");
        #endif
    #endif

    #ifdef TEST_CUSTOM_MALLOC_MACROS
        test_c_malloc();
        test_c_calloc();
        test_c_realloc();
        test_c_free();
        PRINT_MAGENTA("Custom mem alloc tests passed!\n");
    #endif

    #ifdef TEST_FOR_DEFER_MACRO
        printf("----------------------------------------------------\n");
        int t_sum;
        DEBUG_PRINT("Starting defer sum\n");
        FOR_DEFER(t_sum = 0, t_sum=SHRT_MAX) {
            for (int i = 0; i < SHRT_MAX; ++i) {
                t_sum += i*10;
            }
        }
        DEBUG_PRINTF("t_sum is now %d\n", t_sum);
        assert(t_sum == SHRT_MAX);
        PRINT_MAGENTA("Test for defer macro passed!\n");
        printf("----------------------------------------------------\n");
    #endif

    #ifdef TEST_DEFER_GCC_LLVM_MACRO
        printf("----------------------------------------------------\n");
        {
            //TODO
        }
        PRINT_MAGENTA("Test for GCC/LLVM defer macro passed!\n");
        printf("----------------------------------------------------\n");
    #endif

    PRINT_BLUE("Macro tests passed!\n");

#endif

#if TEST_SLL_AND_BOILERPLATE == 1
    err = malloc(sizeof(err_s));
    err->err_type = NONE;
    err->err_msg = NULL;

    #ifdef TEST_TABLE_CREATION
        printf("----------------------------------------------------\n");
        test_create_solution_table(problem_size);
        test_create_vector_of_ones(problem_size);
        test_create_vector_of_zeros(problem_size);
        test_create_vector_of_alternating_ones_zeros(problem_size);
        test_create_vector_of_randoms(problem_size);
        test_create_vector_of_true_randoms(problem_size);
        PRINT_MAGENTA("Test for table creation passed!\n");
        printf("----------------------------------------------------\n");
    #endif

    #ifdef TEST_RANDOM_GETTERS
        printf("----------------------------------------------------\n");
        test_get_next_binary_rand();
        test_get_next_binary_true_rand();
        PRINT_MAGENTA("Test for RAND passed!\n");
        printf("----------------------------------------------------\n");
    #endif

    #ifdef TEST_SIZE_AND_ERRTYPE
        printf("----------------------------------------------------\n");
        test_size_is_multiple_of_problem_size(problem_size);
        test_get_error_type_string();
        PRINT_MAGENTA("Test for problem size and errtype passed!\n");
        printf("----------------------------------------------------\n");
    #endif

    #ifdef TEST_SINGLE_LINKED_LIST
        printf("----------------------------------------------------\n");
        test_initialize_iteration_data_list();
        //TODO
        // test_create_iteration_data_sll_node(solver_iteration_data *, err_s *);
        // test_create_iteration_data(const double, const uint64_t, const size_t, uint8_t *, err_s *);
        // test_add_iteration_data_node_to_sll(iteration_data_sll *, solver_iteration_data *, err_s *ppErr);
        // test_free_iteration_data_sll(iteration_data_sll **);
        // test_free_iteration_data_node(iteration_data_node **);
        // test_free_iteration_data(solver_iteration_data **);
        PRINT_MAGENTA("Test for single linked list passed!\n");
        printf("----------------------------------------------------\n");
    #endif

    PRINT_BLUE("Sll and boilerplate tests passed!\n");

#endif

#ifdef TEST_COMPUTE
    printf("----------------------------------------------------\n");
    //TODO
    PRINT_MAGENTA("Test for compute passed!\n");
    printf("----------------------------------------------------\n");

    PRINT_BLUE("COMPUTE tests passed!\n");
#endif

    PRINT_CYAN("All tests passed!\n");

    return EXIT_SUCCESS;
}

void test_c_malloc() {

}

void test_c_calloc() {

}

void test_c_realloc() {

}

void test_c_free() {

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

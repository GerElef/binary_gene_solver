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
#include "globals.h"

uint8_t *(*getCreator(enum Vector_t creator_type))(size_t, err_s *) {
    switch (creator_type) {
        case VECTOR_OF_ZEROS:
            return create_vector_of_zeros;
        case VECTOR_OF_ONES:
            return create_vector_of_ones;
        case VECTOR_OF_ALTERNATING_ONES_ZEROS:
            return create_vector_of_alternating_ones_zeros;
        case VECTOR_OF_RANDOMS:
            return create_vector_of_randoms;
        case VECTOR_OF_TRUE_RANDOMS:
            return create_vector_of_true_randoms;
        default:
            DEBUG_PRINTF("Invalid vector type initialization switch val %d\n", creator_type);
            exit(EXIT_FAILURE);
    }
}

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

    return sol;
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

    return sol;
}

//returns the next binary random
uint8_t get_next_binary_rand(){
    return rand()%2 == 0;
}

//returns the next binary random
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
//TODO add debug prints on all SLL functions
iteration_data_sll *initialize_iteration_data_list(err_s *ppErr) {
    DEBUG_PRINTF("Creating iteration single linked list with ppErr \tp: %p\n", (void *) ppErr);

    iteration_data_sll *iter_sll = malloc(sizeof(iteration_data_sll));
    if (!iter_sll) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        DEBUG_PRINTF("Malloc failed with iter_sll \tp: %p and err %s\n", (void *) iter_sll, ppErr->err_msg);
        return NULL;
    }

    DEBUG_PRINTF("Created iteration single linked list with \tp: %p\n", (void *) iter_sll);

    iter_sll->head=NULL;
    iter_sll->size=0;

    return iter_sll;
}

iteration_data_node *create_iteration_data_sll_node(solver_iteration_data *iter_data, err_s *ppErr) {
    DEBUG_PRINTF("Creating iteration data node with ppErr \tp: %p and iter_data \tp: %p\n", (void *) ppErr, (void *)iter_data);

    iteration_data_node *iter_data_node = malloc(sizeof(iteration_data_node));
    if (!iter_data_node) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        DEBUG_PRINTF("Malloc failed with iter_data_node \tp: %p and err %s\n", (void *) iter_data_node, ppErr->err_msg);
        return NULL;
    }

    DEBUG_PRINTF("Created iteration data node with ppErr \tp: %p and iter_data_node \tp: %p\n", (void *) ppErr, (void *)iter_data_node);

    iter_data_node->data = iter_data;
    iter_data_node->next = NULL;

    return iter_data_node;
}

solver_iteration_data *create_iteration_data(const double fitness, const uint64_t score, const size_t p_size, uint8_t *input_table, err_s *ppErr) {
    DEBUG_PRINTF("Creating solver_iteration_data with \tfitness: %f,\tscore: %lu,\tp_size: %zu,\tinput_table p: %p\tppErr \tp: %p\n",
                 fitness, score, p_size, (void *)input_table, (void *) ppErr);

    solver_iteration_data *data = malloc(sizeof(solver_iteration_data));
    if (!data) {
        ppErr->err_type = MEMORY_ALLOCATION_FAILED;
        char *errt = get_error_type_string(MEMORY_ALLOCATION_FAILED);
        ppErr->err_msg = calloc(ERROR_MAX_MSG_SIZE, sizeof(char));
        sprintf(ppErr->err_msg, "Error type %s\tline %d\tfile %s", errt, __LINE__, __FILE__);
        DEBUG_PRINTF("Malloc failed with solver_iteration_data \tp: %p and err %s\n", (void *) data, ppErr->err_msg);
        return NULL;
    }

    DEBUG_PRINTF("Created solver_iteration_data with \tsolver_iteration_data p: %p\n", (void *) data);

    data->size = p_size;
    data->fitness = fitness;
    data->score = score;
    data->input_table = input_table;

    return data;
}

void add_iteration_data_node_to_sll(iteration_data_sll *sll, solver_iteration_data *iter_data, err_s *ppErr) {
    // if head is initialized...
    DEBUG_PRINTF("SLL p: %p,\titer_data p: %p,\t ppErr p: %p\n", (void *)sll, (void *)iter_data, (void *)ppErr);
    DEBUG_PRINTF("SLL size %zu\n", sll->size);
    if (sll->size != 0) {
        iteration_data_node *iter_node = sll->head;
        DEBUG_PRINTF("Starting iterations with iter_node p: %p\n", (void *) iter_node);
        for (size_t i = 0; i < sll->size - 1; ++i) {
            iter_node = iter_node->next;
        }
        DEBUG_PRINTF("Stopped iterations with iter_node p: %p\n", (void *) iter_node);
        iter_node->next = create_iteration_data_sll_node(iter_data, ppErr);
        DEBUG_PRINTF("Created new iter_node->next p: %p\n", (void *) iter_node->next);
    } else {
        DEBUG_PRINTF("SLL head before alloc p: %p\n", (void *)sll->head);
        sll->head = create_iteration_data_sll_node(iter_data, ppErr);
        DEBUG_PRINTF("SLL head after alloc p: %p\n", (void *)sll->head);
    }

    if (ppErr->err_type != NONE) {
        return;
    }

    sll->size++;
}

void free_iteration_data_sll(iteration_data_sll **pp_iteration_data_sll) {
    iteration_data_node *iterator = (*pp_iteration_data_sll)->head;
    iteration_data_node *next_item = NULL;
    size_t s = (*pp_iteration_data_sll)->size;
    DEBUG_PRINTF("Starting with iterator p: %p\t size: %zu\n", (void *)iterator, s);
    for (size_t i = 0; i < s; ++i) {
        next_item = iterator->next;
        DEBUG_PRINTF("Got next_item p: %p\t freeing iterator data node p: %p\n", (void *) iterator, (void *)iterator);
        free_iteration_data_node(&iterator);
        iterator = next_item;
    }

    free(*pp_iteration_data_sll);
    *pp_iteration_data_sll = NULL;

}

void free_iteration_data_node(iteration_data_node **pp_iter_data_node) {
    iteration_data_node *p_iter = *pp_iter_data_node;
    DEBUG_PRINTF("Freeing iteration_data_node p: %p\tsolver_iteration_data p: %p\n", (void *) p_iter, (void *)p_iter->data);
    free_iteration_data(&(p_iter->data));
    DEBUG_PRINTF("Freed iteration_data_node p: %p\tsolver_iteration_data p: %p\n", (void *) p_iter, (void *)p_iter->data);
    p_iter = NULL;
}

void free_iteration_data(solver_iteration_data **pp_solver_iteration_data) {
    solver_iteration_data *p_solver = *pp_solver_iteration_data;
    DEBUG_PRINTF("Freeing p_solver->input_table p: %p,\tp_solver p:%p\n", (void *)p_solver->input_table, (void *)p_solver);
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

void *c_malloc(size_t b, const int line, const char *file, const char *func) {
    return NULL;
}

void *c_calloc(size_t n, size_t n_size, const int line, const char *file, const char *func) {
    return NULL;
}

void *c_realloc(void *p, size_t new_size, const int line, const char *file, const char *func) {
    return NULL;
}

void c_free(void *p, const int line, const char *file, const char *func) {
    return;
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


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

// TODO check everything works correctly

uint64_t MAX_ITERATIONS;

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

    // TODO everything else here
}

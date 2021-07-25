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

// TODO create mem arena bind/macro?
// TODO create custom malloc maybe?
// TODO parallelize with openmp?
// TODO add visualizations etc.
//  we might have to brute force a small problem or two to create thw "y" dimension; how can we flatten the n-th
//  dimensional problem into a 2d or 3d plot?

uint64_t MAX_ITERATIONS;

int main(int argc, char *argv[]) {
    err_s *err = malloc(sizeof(err_s));
    err->err_type = NONE;
    err->err_msg = NULL;

    srand(SEED); // calls to lrand48(); will now work correctly

    enum Vector_t creator_type = VECTOR_OF_ZEROS;
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

    uint8_t * (*creator)(size_t, err_s *) = getCreator(creator_type);

    uint8_t *solution_table = create_solution_table(creator, problem_size, err);

    DEBUG_ARENA(
            for (size_t i = 0; i < problem_size; ++i) {
                printf("%d", solution_table[i]);
                if ((i+1) % MULTIPLE_OF_PROBLEM_SIZE == 0) printf("\n");
            }
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

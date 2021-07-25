#ifndef BINARY_GENE_SOLVER_MACROS_H
#define BINARY_GENE_SOLVER_MACROS_H

#define PRETTY_PRINT    1
#define CUSTOM_MALLOC   1
#define __DEBUG__       1

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
    #include <stdio.h>
    #define PRINTF_RED(x, ...)      printf(ANSI_COLOR_RED     x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_GREEN(x, ...)    printf(ANSI_COLOR_GREEN   x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_YELLOW(x, ...)   printf(ANSI_COLOR_YELLOW  x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_BLUE(x, ...)     printf(ANSI_COLOR_BLUE    x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_MAGENTA(x, ...)  printf(ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET, __VA_ARGS__)
    #define PRINTF_CYAN(x, ...)     printf(ANSI_COLOR_CYAN    x ANSI_COLOR_RESET, __VA_ARGS__)

    #define PRINT_RED(x)            printf(ANSI_COLOR_RED     x ANSI_COLOR_RESET)
    #define PRINT_GREEN(x)          printf(ANSI_COLOR_GREEN   x ANSI_COLOR_RESET)
    #define PRINT_YELLOW(x)         printf(ANSI_COLOR_YELLOW  x ANSI_COLOR_RESET)
    #define PRINT_BLUE(x)           printf(ANSI_COLOR_BLUE    x ANSI_COLOR_RESET)
    #define PRINT_MAGENTA(x)        printf(ANSI_COLOR_MAGENTA x ANSI_COLOR_RESET)
    #define PRINT_CYAN(x)           printf(ANSI_COLOR_CYAN    x ANSI_COLOR_RESET)
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

#if CUSTOM_MALLOC == 1
    #include <unistd.h>
    #include <sys/mman.h>

    #define CMALLOC(b)              c_malloc(b, __LINE__, __FILE__, __func__)
    #define CCALLOC(n, n_size)      c_calloc(n, n_size, __LINE__, __FILE__, __func__)
    #define CREALLOC(p, new_size)   c_realloc(p, new_size, __LINE__, __FILE__, __func__)
    #define CFREE(p)                c_free(p, __LINE__, __FILE__, __func__)

    #define malloc(b)               CMALLOC(b)
    #define calloc(n, n_size)       CCALLOC(n, n_size)
    #define realloc(p, new_size)    CREALLOC(p, new_size)
    #define free(p)                 CFREE(p)
#else
    //these are probably not needed, but these are here for posterity and continuity

    #include <stdlib.h>
    #define malloc(b)               malloc(b)
    #define calloc(n, n_size)       calloc(n, n_size)
    #define realloc(p, new_size)    realloc(p, new_size)
    #define free(p)                 free(p)
#endif

//https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#define DEBUG_ARENA(...)     do { if (__DEBUG__) { __VA_ARGS__ } } while(0)
#define DEBUG_PRINTF(x, ...) DEBUG_ARENA(PRINTF_YELLOW("%s:%d:%s():\t" x ANSI_COLOR_RESET, __FILE__, __LINE__, __func__, __VA_ARGS__);)
#define DEBUG_PRINT(x)       DEBUG_ARENA(PRINT_YELLOW(ANSI_COLOR_YELLOW "%s:%d:%s():\t" x ANSI_COLOR_RESET, __FILE__, __LINE__, __func__);)

//https://stackoverflow.com/questions/1082192/how-to-generate-random-variable-names-in-c-using-macros/17624752#17624752
#define VAR_CONCAT(a, b) VAR_CONCAT_INNER(a, b)
#define VAR_CONCAT_INNER(a, b) a ## b
#define itervar(base) VAR_CONCAT(base, __LINE__)
//https://news.ycombinator.com/item?id=27137893
#define FOR_DEFER(pre, post) for (int itervar(i) = ((pre), 0); !itervar(i)++; (post))

#endif //BINARY_GENE_SOLVER_MACROS_H

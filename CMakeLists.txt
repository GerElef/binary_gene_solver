cmake_minimum_required(VERSION 3.19)
project(binary_gene_solver C)

set(CMAKE_C_STANDARD 11)

#one of these must be active at a time... god, most of these -- if not all -- don't work on windows
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g -lasan")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -g")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread -g")
#this doesn't work with GCC
#gcc flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Winit-self -Wlogical-op -Wmissing-include-dirs -Wredundant-decls -Wshadow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-overflow=5 -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-show-option -fno-exceptions")

add_executable(binary_gene_solver main.c compute.h macros.h compute.c globals.h)

add_executable(binary_gene_solver_tests tests.c compute.h macros.h compute.c globals.h)
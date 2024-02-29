cmake_minimum_required(VERSION 3.10)

project(bigintlib)

# Add the source files for the static library
add_library(bigint STATIC
    big-int-lib/bigint.c
    big-int-lib/utils.c
    big-int-lib/sum.c
    big-int-lib/sub.c
    big-int-lib/mul.c
    big-int-lib/div.c
)

# Add the source files for the tests executable
add_executable(tests
    tests/main.c
)

# Link the static library to the tests executable
target_link_libraries(tests bigint)

# Set the output directory for the tests executable
set_target_properties(tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/tests")

# Set the output directory for the static library
set_target_properties(bigint PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/big-int-lib")

# Add compiler flags
target_compile_options(bigint PRIVATE -Wall -g)
target_compile_options(tests PRIVATE -Wall -g)

enable_testing()
add_test(NAME bigintlib_tests COMMAND tests)

# Add Clang format
file(GLOB_RECURSE ALL_SOURCE_FILES *.c *.h)
add_custom_target(format COMMAND clang-format -style=Google -i ${ALL_SOURCE_FILES})

# Add Valgrind
add_custom_target(memcheck COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose "${CMAKE_CURRENT_SOURCE_DIR}/build/tests/tests" DEPENDS tests)
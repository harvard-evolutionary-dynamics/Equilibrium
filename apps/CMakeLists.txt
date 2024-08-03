################# BEGIN diversity_counts ####################

add_executable(diversity_counts diversity_counts.cc)
target_link_libraries(diversity_counts PRIVATE equilibrium gflags::gflags prettyprint OpenMP::OpenMP_CXX)

# Cross-platform compiler lints
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
        OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(diversity_counts PRIVATE
            -Wall
            -Wextra
            -Wswitch
            -Wconversion
            -Wparentheses
            -Wfloat-equal
            -Wzero-as-null-pointer-constant
            -Wpedantic
            -pedantic
            -pedantic-errors)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    target_compile_options(diversity_counts PRIVATE
            /W3)
endif ()

# add_custom_command(
#         TARGET diversity_counts
#         PRE_BUILD
#         COMMAND ${CMAKE_COMMAND} -DSRC_DIR="${CMAKE_SOURCE_DIR}/data" -DDEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/data" -P "${CMAKE_SOURCE_DIR}/cmake/configure_files.cmake"
# )

################# END diversity_counts ####################

################# BEGIN simulation_history ####################

add_executable(simulation_history simulation_history.cc)
target_link_libraries(simulation_history PRIVATE equilibrium gflags::gflags prettyprint OpenMP::OpenMP_CXX)

# Cross-platform compiler lints
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"
        OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(simulation_history PRIVATE
            -Wall
            -Wextra
            -Wswitch
            -Wconversion
            -Wparentheses
            -Wfloat-equal
            -Wzero-as-null-pointer-constant
            -Wpedantic
            -pedantic
            -pedantic-errors)
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    target_compile_options(simulation_history PRIVATE
            /W3)
endif ()

# add_custom_command(
#         TARGET simulation_history
#         PRE_BUILD
#         COMMAND ${CMAKE_COMMAND} -DSRC_DIR="${CMAKE_SOURCE_DIR}/data" -DDEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/data" -P "${CMAKE_SOURCE_DIR}/cmake/configure_files.cmake"
# )

################# END simulation_history ####################
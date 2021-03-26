cmake_minimum_required(VERSION 3.6)
project(iz-2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDART_REQURED ON)


set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Wextra -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -Wextra -Wpedantic -ftest-coverage")

add_library( lib_search_by_files_in_dir STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/project/lib_search_by_files_in_dir/search_by_files_in_dir.c
        project/lib_search_by_files_in_dir/search_word_in_file.c)

target_include_directories( lib_search_by_files_in_dir
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/include
        )

add_executable(main
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/main.c)

target_include_directories(main
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/include
        )

target_link_libraries(main lib_search_by_files_in_dir)

add_executable(tests
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/tests.cpp
        )

target_include_directories(tests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/include
        ${GTEST_INCLUDE_DIRS}
        )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest)
find_package(Threads)

target_link_libraries(tests lib_search_by_files_in_dir gtest ${CMAKE_THREAD_LIBS_INIT})
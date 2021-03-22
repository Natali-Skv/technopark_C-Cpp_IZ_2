cmake_minimum_required(VERSION 3.6)
project(iz-2)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDART_REQURED ON)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

#add_library(word_lib STATIC
#        ${CMAKE_CURRENT_SOURCE_DIR}/project/word_lib/word.c
#        )
#
#target_include_directories(word_lib
#        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/include
#        )

add_executable(main
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/main.c project/include/search_by_files_in_dir.h)

target_include_directories(main
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/include
        )

#target_link_libraries(IZ-1 word_lib)
#
#add_executable(tests
#        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
#        )
#
#target_include_directories(tests
#        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/project/include
#        #${GTEST_INCLUDE_DIRS}
#        )
#
##add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/gtest)
#add_subdirectory(gtest)
#find_package(Threads)
#
#target_link_libraries(tests word_lib gtest ${CMAKE_THREAD_LIBS_INIT})
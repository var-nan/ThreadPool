cmake_minimum_required(VERSION 3.26)
project(thread_pool C)

set(CMAKE_C_STANDARD 11)
add_link_options(-pthread)

add_executable(thread_pool main.c
        tpool_pq.h
        thread_pool.h
        thread_pool.c
        tpool_pq.c)

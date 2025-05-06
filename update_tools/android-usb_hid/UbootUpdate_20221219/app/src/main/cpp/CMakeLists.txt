cmake_minimum_required(VERSION 3.18.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# ndk version >=21, need to add '-static-openmp', or remove it is better
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -static-openmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -static-openmp")

add_library( # Sets the name of the library.
        native-lib
        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        jl_bootloader_jni.cpp
        jl_bootloader.cpp
        )

target_link_libraries( # Specifies the target library.
        native-lib
        log)
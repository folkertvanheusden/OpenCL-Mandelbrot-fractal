# (C) 2025-2026 by folkert van heusden <folkert@komputilo.nl>, MIT license
cmake_minimum_required(VERSION 3.9.4)

project(opencl-mandelbrot VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_compile_options(-Wall -pedantic)

add_executable(opencl-mandelbrot
	kernel.cpp
	test.cpp
)

include(FindPkgConfig)

pkg_check_modules(LIBOPENCL OpenCL)
target_link_libraries(opencl-mandelbrot ${LIBOPENCL_LIBRARIES})
target_include_directories(opencl-mandelbrot PUBLIC ${LIBOPENCL_INCLUDE_DIRS})
target_compile_options(opencl-mandelbrot PUBLIC ${LIBOPENCL_CFLAGS_OTHER})

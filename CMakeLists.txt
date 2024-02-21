cmake_minimum_required(VERSION 3.10)
project(COMP7402_Assignment5)

# Include directories
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/src")
#set(OPENSSL_USE_STATIC_LIBS TRUE)


# Find all source files in the src directory
file(GLOB_RECURSE PROJECT_FILES
        "${INCLUDE_DIR}/*.h"
        "${SOURCE_DIR}/*.cpp"
        )

add_executable("Feistel" ${PROJECT_FILES})

target_include_directories("Feistel" PRIVATE ${INCLUDE_DIR})
target_compile_features("Feistel" PRIVATE cxx_std_17)


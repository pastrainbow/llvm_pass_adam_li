cmake_minimum_required(VERSION 3.13)
project(CountFnBBPass)

# Find an installed LLVM and load its CMake configuration
find_package(LLVM REQUIRED CONFIG)

# Build the shared library from source
add_library(CountFnBBPass MODULE CountFnBBPass.cpp)

# Add LLVM header paths to our library so we can include LLVM related header 
# files
target_include_directories(CountFnBBPass PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(CountFnBBPass PRIVATE ${LLVM_DEFINITIONS})

# Remove the default lib prefix so our library name is CountFnBBPass.so, and
# output the library file in project root directory
set_target_properties(CountFnBBPass PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
    PREFIX ""
)


# Link to the LLVM shared library
set(LLVM_LINK_LLVM_DYLIB ON)
target_link_libraries(CountFnBBPass PRIVATE LLVM)

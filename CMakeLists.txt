cmake_minimum_required(VERSION 2.8.8)
SET(CMAKE_BUILD_TYPE Debug)
project(agaLang)

set(LLVM_PATH llvm)
set(LLVM_DIR llvm/cmake/modules/CMakeFiles)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

FILE(GLOB_RECURSE SRC_ALL "src/*.cpp")
FILE(GLOB_RECURSE INC_ALL "include/*.h")
FILE(GLOB_RECURSE LLVM_INC_ALL "llvm-3.8.0.src/include/*.h")
FILE(GLOB_RECURSE TEST_ALL "test/*.*")
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
include_directories(include llvm/include llvm-3.8.0.src/include ${LLVM_INCLUDE_DIRS})

add_definitions(${LLVM_DEFINITIONS})
ADD_DEFINITIONS(-std=c++14 -D_DEBUG)

add_executable(agaLang ${SRC_ALL} ${INC_ALL} ${TEST_ALL})

# Find the libraries that correspond to the LLVM components
# that we wish to use
llvm_map_components_to_libnames(llvm_libs core)
#llvm_map_components_to_libnames(llvm_libs core interpreter x86codegen)

# Link against LLVM libraries
target_link_libraries(agaLang ${llvm_libs})

cmake_minimum_required(VERSION 2.6)
project(agalang)

add_executable(agalang src/agaCompiler.cpp src/agaCodeGenerator.cpp src/agaParser.cpp src/agaToken.cpp src/agaException.cpp src/agaLogger.cpp src/agaLexer.cpp src/agaMain.cpp)

install(TARGETS agalang RUNTIME DESTINATION bin)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(include)
include_directories(include)

ADD_DEFINITIONS(-std=c++0x)
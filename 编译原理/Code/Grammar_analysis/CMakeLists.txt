cmake_minimum_required(VERSION 3.22) 

project(Grammar_analysis)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use built-in variable for source directory
include_directories(include) 

# Add executable target with more concise syntax
add_executable(${PROJECT_NAME} 
    src/TESTSYM.CPP 
    src/SYMBOL.CPP 
    src/ExpressionCalculator.cpp 
)
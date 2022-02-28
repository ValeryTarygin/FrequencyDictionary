cmake_minimum_required (VERSION 3.12)
project(FrequencyDictionary)

add_executable(${PROJECT_NAME} src/frequency_dictionary.cpp)

target_compile_features (${PROJECT_NAME} PRIVATE cxx_std_17)
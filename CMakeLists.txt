cmake_minimum_required(VERSION 3.7)
project(Montador_LigadorSB)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp analisadorLexico.cpp preprocessador.cpp "analisador sintatico.cpp" "analisador semantico.cpp" ligador.cpp montador.cpp)
add_executable(Montador_LigadorSB ${SOURCE_FILES} analisadorLexico.cpp)
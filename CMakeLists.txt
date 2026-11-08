cmake_minimum_required(VERSION 3.20)
project(ToryumCoreAI VERSION 0.1.0 LANGUAGES CXX)

# Exige o padrão moderno C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configurações de compilação de alta performance

# Define o executável do Kernel
add_executable(toryum_kernel 
    src/main.cpp
)

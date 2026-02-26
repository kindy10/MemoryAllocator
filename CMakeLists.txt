cmake_minimum_required(VERSION 3.30)
project(MemoryAllocator)

set(CMAKE_CXX_STANDARD 20)
#Include le dossier des headers
include_directories(include)

#Definir l'executable et les fichiers necessaires
add_executable(MemoryAllocator
        main.cpp
        src/allocator.cpp
        include/allocator.hpp
        src/my_free.cpp
)


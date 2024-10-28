cmake_minimum_required(VERSION 3.22)

project(PROJET_CPP)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pg -g -O3 -Wall") # -O3 for agressive optimization

## Définir la localisation des entêtes.
## include_directories sera propager à l'ensemble du projet
include_directories(include)

# Add your source files and include directory
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add your source files to the project
add_executable(main ${SOURCES})

## Parcours les sous répertoires contenant les définitions (.cxx)
## On commence par créer une bibliothèque
add_subdirectory(src)

## On créée les tests pour la bibliothèque.
## Ces tests sont unitaires ou fonctionnels
add_subdirectory(test)


## On récupère les sources de gtest
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.14.0.zip
    PREFIX gtest
    DOWNLOAD_EXTRACT_TIMESTAMP true
    INSTALL_COMMAND ""
)

set(GTEST_INCLUDE_DIRS gtest/src/googletest/googletest/include)
set(GTEST_LIBRARIES gtest/src/googletest-build/lib/libgtest.a)

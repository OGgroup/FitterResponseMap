cmake_minimum_required(VERSION 3.11)
project(FitterResponseMap VERSION 0.0.1
                          DESCRIPTION "Fake Reconstruction from Response Map"
                          LANGUAGES C CXX)

file( TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  message(FATAL_ERROR "Cannot build in source directory")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(THREADS_PREFER_PTHREAD_FLAT ON)

add_definitions( -DDATA_DIRECTORY="${CMAKE_INSTALL_PREFIX}/share/" )

find_package(Threads REQUIRED)
find_package(ROOT CONFIG REQUIRED COMPONENTS Minuit2)
include(${ROOT_USE_FILE})

set( FRP_BINARY_DIR ${PROJECT_BINARY_DIR}/bin CACHE PATH "Executables" )
set( FRP_LIBRARY_DIR ${PROJECT_BINARY_DIR}/lib CACHE PATH "Libraries" )
set( FRP_INCLUDE_DIR ${PROJECT_BINARY_DIR}/include CACHE PATH "Headers" )

add_subdirectory (src)
add_subdirectory (demo)

install(DIRECTORY data/ DESTINATION ${CMAKE_INSTALL_PREFIX}/share
  PATTERN "data/*")

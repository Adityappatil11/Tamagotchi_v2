cmake_minimum_required(VERSION 3.14)
project(Tamagotchi_v2)

#provide option to build static or dynamic libraries
option(BUILD_SHARED_LIBS "build shared libraries" OFF)

#adding core application library
add_subdirectory(app)

#adding testing for local
enable_testing()
add_subdirectory(tests)

#output message information
if(BUILD_SHARED_LIBS)
    message(STATUS ">>> Building a Shared Library")
else()
    message(STATUS ">>> Building a Static Library")
endif()
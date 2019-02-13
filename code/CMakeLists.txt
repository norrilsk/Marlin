cmake_minimum_required(VERSION 3.1)
project(Marlin)


set(dir ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(build_type debug)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH ${dir}/bin/${build_type})
set(LIBRARY_OUTPUT_PATH ${dir}/bin/${build_type}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir}/bin/${build_type})
set(CMAKE_BUILD_FILES_DIRECTORY ${dir}/bin/${build_type})
set(CMAKE_BUILD_DIRECTORY ${dir}/bin/${build_type})
set(CMAKE_BINARY_DIR  ${dir}/bin/${build_type})
set(EXECUTABLE_OUTPUT_PATH ${dir}/bin/${build_type})
set(CMAKE_CACHEFILE_DIR ${dir}/bin/${build_type})
set(CMAKE_FILES_DIRECTORY ${dir}/bin/${build_type}/CMakeFiles)


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(include)

find_library(LIBELF_LIBRARY elf)
if(NOT LIBELF_LIBRARY)
    message(SEND_ERROR "Failed to find libelf")
    return()
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(${PROJECT_NAME} ElfMarlin.cpp Marlin.cpp Marlin.hpp Main.cpp ElfMarlin.hpp Memory.cpp Memory.hpp MMU.cpp
MMU.hpp Log.cpp Log.hpp Config.cpp Config.hpp)

target_link_libraries(${PROJECT_NAME} ${LIBELF_LIBRARY} )


cmake_minimum_required(VERSION 2.6)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

#set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/.. )

project (fmi)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(~/include) #SDSL headers are here

LINK_DIRECTORIES(~/lib) #SDSL lib are here

message("Building in ${CMAKE_BUILD_TYPE} mode")

set(CMAKE_CXX_FLAGS "--std=c++11")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -g")
set(CMAKE_CXX_FLAGS_RELEASE "-g -ggdb -Ofast -fstrict-aliasing -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g -ggdb -Ofast -fstrict-aliasing -march=native")

add_executable(fmi-build fmi-build.cpp)
TARGET_LINK_LIBRARIES(fmi-build sdsl)
TARGET_LINK_LIBRARIES(fmi-build divsufsort)
TARGET_LINK_LIBRARIES(fmi-build divsufsort64)

add_executable(fmi-search fmi-search.cpp)
TARGET_LINK_LIBRARIES(fmi-search sdsl)
TARGET_LINK_LIBRARIES(fmi-search divsufsort)
TARGET_LINK_LIBRARIES(fmi-search divsufsort64)

add_executable(genpatterns genpatterns.cpp)
TARGET_LINK_LIBRARIES(genpatterns sdsl)
TARGET_LINK_LIBRARIES(genpatterns divsufsort)
TARGET_LINK_LIBRARIES(genpatterns divsufsort64)

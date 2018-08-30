cmake_minimum_required(VERSION 3.4)
PROJECT (BIONETIO)

# set the models for external library
message(status "Setting G++ flags")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -Wall -Wno-deprecated -DIL_STD")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -Wall -Wno-deprecated -DIL_STD -DNDEBUG")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/src
)

# add the libraries you need below
# if (UNIX AND NOT APPLE)
# elseif(APPLE)
# endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# automatically grab all the source files
#file(GLOB SOURCES src/*.cpp)
#file(GLOB HEADERS include/*.h)

set(SOURCES
    include/CommonClass.h
    include/BioNetIOXML.h
    )

set(SOURCES
    src/main.cpp
    src/BioNetIOXML.cpp
    )

add_executable (bionetio ${SOURCES})
target_link_libraries (bionetio tinyxml sbml)

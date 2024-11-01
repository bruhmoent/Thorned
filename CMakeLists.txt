cmake_minimum_required(VERSION 3.15)

project(ThornedLibrary VERSION 1.0 LANGUAGES CXX)

option(DEV_MODE "Enable development mode" OFF)

set(BUILD_SHARED_LIBS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /D_WINDOWS /W3 /utf-8")

set(CMAKE_GENERATOR_PLATFORM x64)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

if(NOT SFML_FOUND)
    message(FATAL_ERROR "SFML not found. Please ensure it is installed correctly.")
endif()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
    "${PROJECT_SOURCE_DIR}/GUI/*.cpp"
    "${PROJECT_SOURCE_DIR}/GUI/components/*.cpp"
    "${PROJECT_SOURCE_DIR}/GUI/form/*.cpp"
    "${PROJECT_SOURCE_DIR}/XML/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS 
    "${PROJECT_SOURCE_DIR}/GUI/*.hpp"
    "${PROJECT_SOURCE_DIR}/GUI/components/*.hpp"
    "${PROJECT_SOURCE_DIR}/GUI/form/*.hpp"
    "${PROJECT_SOURCE_DIR}/XML/*.hpp"
    "${PROJECT_SOURCE_DIR}/XML/*.h"
)

add_library(ThornedLibrary STATIC ${SOURCES} ${HEADERS})

target_link_libraries(ThornedLibrary PRIVATE sfml-system sfml-window sfml-graphics sfml-audio)

target_include_directories(ThornedLibrary PRIVATE 
    ${SFML_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(ThornedLibrary PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}"
    OUTPUT_NAME "Thorned"
)

if(DEV_MODE)
    add_executable(ThornedApp main.cpp)

    target_link_libraries(ThornedApp PRIVATE ThornedLibrary sfml-system sfml-window sfml-graphics sfml-audio)

    target_include_directories(ThornedApp PRIVATE 
        ${SFML_INCLUDE_DIRS}
        ${PROJECT_SOURCE_DIR}/include
    )

    set_target_properties(ThornedApp PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}"
    )
endif()

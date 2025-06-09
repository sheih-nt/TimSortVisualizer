cmake_minimum_required(VERSION 3.15)
project(TimSortVisualizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Настройки для Windows
if(WIN32)
    add_compile_definitions(_WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
else()
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# Поиск raylib и raygui
find_package(raylib REQUIRED)
find_package(raygui REQUIRED)

# Визуализатор TimSort
add_library(timsort
    include/timsort.hpp src/timsort.cpp
    include/visualizer.hpp src/visualizer.cpp)
target_include_directories(timsort PRIVATE include)
target_link_libraries(timsort PRIVATE raylib raygui)

# Главная точка входа
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE timsort raylib raygui)

# Копирование файла данных (опционально)
if(EXISTS "${CMAKE_SOURCE_DIR}/numbers.txt")
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_SOURCE_DIR}/numbers.txt
                ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/numbers.txt
    )
endif()

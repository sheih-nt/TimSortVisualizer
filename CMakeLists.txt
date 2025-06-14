cmake_minimum_required(VERSION 3.15)
project(TimSortVisualizer LANGUAGES CXX)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lraylib -lraygui -Wl,-Bdynamic")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включить статическую линковку
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# Пути к библиотекам raylib и raygui
set(LIB_RAYLIB ${CMAKE_SOURCE_DIR}/lib/libraylib.a)
set(LIB_RAYGUI ${CMAKE_SOURCE_DIR}/lib/libraygui.a)

# Найти системные зависимости
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

add_library(raylib STATIC IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${LIB_RAYLIB}
    INTERFACE_LINK_LIBRARIES "OpenGL::GL;Threads::Threads;dl;m;pthread"
)

add_library(raygui STATIC IMPORTED)
set_target_properties(raygui PROPERTIES
    IMPORTED_LOCATION ${LIB_RAYGUI}
    INTERFACE_LINK_LIBRARIES raylib
)

# Визуализатор TimSort
add_library(timsort
    include/timsort.hpp src/timsort.cpp
    include/visualizer.hpp src/visualizer.cpp)
target_include_directories(timsort PRIVATE include)
target_link_libraries(timsort PRIVATE raylib raygui)

# Главная точка входа
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE timsort)

# Копирование файла данных
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/numbers.txt
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/numbers.txt
)

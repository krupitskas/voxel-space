cmake_minimum_required(VERSION 3.19)
project(opendelta)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options(
            $<$<CONFIG:>:/MT> #---------|
            $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
            $<$<CONFIG:Release>:/MT> #--|
    )
endif()

add_executable(opendelta main.cpp)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
target_link_libraries(opendelta PRIVATE sfml-system sfml-graphics sfml-window)

cmake_minimum_required(VERSION 3.19)

project(opendelta)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/3rdparty")

include(deps)

set(CMAKE_CXX_STANDARD 20)

add_executable(opendelta src/main.cpp)

target_link_libraries(opendelta PRIVATE sfml-system sfml-graphics sfml-window ) #  ImGui-SFML::ImGui-SFML
target_include_directories(opendelta PRIVATE 3rdparty ${imgui_SOURCE_DIR})

add_custom_command(
        TARGET opendelta
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data)

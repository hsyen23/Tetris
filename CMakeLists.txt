cmake_minimum_required(VERSION 3.0)

project(sfml_test)

set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(sfml_test main.cpp)

target_link_libraries(sfml_test sfml-graphics sfml-audio)

cmake_minimum_required(VERSION 3.20)
project(lichii_ps5_custom LANGUAGES CXX)

option(LICHII_PS5 "Build the native PS5 homebrew target" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(lichii_core STATIC
    src/pad_input.cpp
    src/menu_model.cpp
)
target_include_directories(lichii_core PUBLIC include)

if(LICHII_PS5)
    add_compile_definitions(LICHII_PS5)
    add_executable(eboot
        src/main.cpp
        src/ps5_pad_backend.cpp
    )
    target_link_libraries(eboot PRIVATE lichii_core)
else()
    add_executable(host_demo src/main.cpp)
    target_link_libraries(host_demo PRIVATE lichii_core)

    enable_testing()
    add_executable(lichii_tests tests/test_menu.cpp)
    target_link_libraries(lichii_tests PRIVATE lichii_core)
    add_test(NAME menu_and_pad COMMAND lichii_tests)
endif()

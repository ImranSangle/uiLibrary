
cmake_minimum_required(VERSION 3.5)

project(uiLibrary)

enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-DDEBUG)
endif()

add_library(uiLibrary STATIC 
src/button.cpp
src/static_element.cpp
src/toggleButton.cpp
src/checkbox.cpp
src/input.cpp
src/frame.cpp
src/element.cpp
src/linear_layout.cpp
src/group.cpp
src/progressbar.cpp
src/slider.cpp
)

target_include_directories(uiLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(uiLibrary PUBLIC gdi32 gdiplus)

add_subdirectory(test)

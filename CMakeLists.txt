
cmake_minimum_required(VERSION 3.5)

project(uiLibrary)

enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
add_definitions(-DDEBUG)
endif()

add_library(uiLibrary STATIC 
ui/src/button.cpp
ui/src/static_element.cpp
ui/src/toggleButton.cpp
ui/src/checkbox.cpp
ui/src/input.cpp
ui/src/frame.cpp
ui/src/element.cpp
ui/src/linear_layout.cpp
ui/src/group.cpp
ui/src/progressbar.cpp
ui/src/slider.cpp
)

target_include_directories(uiLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ui/include)

target_link_libraries(uiLibrary PUBLIC gdi32 gdiplus)

add_subdirectory(test)

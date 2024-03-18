
cmake_minimum_required(VERSION 3.5)

project(uiLibrary)

add_library(uiLibrary STATIC 
ui/src/button.cpp
ui/src/input.cpp
ui/src/frame.cpp
ui/src/element.cpp
)

target_include_directories(uiLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ui/include)

target_link_libraries(uiLibrary PUBLIC gdi32 gdiplus)


add_executable(window ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp)
target_link_libraries(window PRIVATE uiLibrary)

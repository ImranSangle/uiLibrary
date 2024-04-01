

add_executable(uiTest window.cpp)

target_link_libraries(uiTest PRIVATE uiLibrary)

add_test(NAME uiTest COMMAND uiTest)

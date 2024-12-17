

add_executable(uiTest test.cpp)

target_link_libraries(uiTest PRIVATE uiLibrary)

add_test(NAME uiTest COMMAND uiTest)

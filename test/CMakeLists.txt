set(TEST_TARGET_NAME test_firefly)
add_executable(${TEST_TARGET_NAME} test_main.cpp)
target_link_libraries(${TEST_TARGET_NAME} PRIVATE firefly cimmerian)

if(MSVC)
  target_compile_options(${TEST_TARGET_NAME} PRIVATE /W4 /EHsc)
else()
  target_compile_options(${TEST_TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

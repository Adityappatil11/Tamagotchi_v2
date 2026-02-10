include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests PetLogicTest.cpp)
target_link_libraries(unit_tests app_logic gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)
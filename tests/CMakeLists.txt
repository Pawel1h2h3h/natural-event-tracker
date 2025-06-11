# tests/CMakeLists.txt

find_package(Catch2 3 REQUIRED)
find_package(cpr REQUIRED)

add_executable(tests
    test_event.cpp
    test_fh.cpp
    test_api.cpp
    test_category.cpp
    ../src/core/apikey.cpp
    ../src/core/category.cpp
    ../src/core/event.cpp
    ../src/core/fileHandler.cpp
)

target_include_directories(tests PRIVATE
    ../src/core
    ../src/include
)

target_link_libraries(tests
    PRIVATE
    Catch2::Catch2WithMain
    cpr::cpr
)

include(CTest)
include(Catch)
catch_discover_tests(tests)

# Enable testing and add tests subdirectory
enable_testing()
add_subdirectory(tests)
# tests/CMakeLists.txt

find_package(Catch2 3 REQUIRED)
find_package(cpr REQUIRED)

add_executable(tests
    test_event.cpp
    test_fh.cpp
    test_api.cpp
    test_category.cpp
    ../src/nasa_proj/src/apikey.cpp
    ../src/nasa_proj/src/category.cpp
    ../src/nasa_proj/src/event.cpp
    ../src/nasa_proj/src/fileHandler.cpp
)

target_include_directories(tests PRIVATE
    ../src
    ../src/nasa_proj/src
)

target_link_libraries(tests
    PRIVATE
    Catch2::Catch2WithMain
    cpr::cpr
)

include(CTest)
include(Catch)
catch_discover_tests(tests)
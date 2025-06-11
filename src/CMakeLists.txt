# src/CMakeLists.txt

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Positioning)

add_library(apikey_lib
            core/apikey.cpp
            core/fileHandler.cpp
            core/event.cpp
            core/category.cpp
            core/models.cpp
            core/mapController.cpp)

target_sources(apikey_lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/mapController.hpp
)


find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(apikey_lib PUBLIC
    Qt6::Core
    Qt6::Positioning
    cpr::cpr
    nlohmann_json::nlohmann_json
)

target_include_directories(apikey_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CPR_INCLUDE_DIRS}
)

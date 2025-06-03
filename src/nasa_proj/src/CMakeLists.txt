set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(apikey_lib
            apikey.cpp
            fileHandler.cpp
            event.cpp
            category.cpp
            volcanoEvent.cpp
            models.cpp)

find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)

target_link_libraries(apikey_lib PUBLIC cpr::cpr)
target_include_directories(apikey_lib PUBLIC ${CPR_INCLUDE_DIRS})

target_link_libraries(apikey_lib PUBLIC nlohmann_json::nlohmann_json)

# (jeśli masz nagłówki, które chcesz upublicznić:)
target_include_directories(apikey_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

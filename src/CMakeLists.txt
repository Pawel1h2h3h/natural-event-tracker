add_library(apikey_lib apikey.cpp)

# (jeśli masz nagłówki, które chcesz upublicznić:)
target_include_directories(apikey_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

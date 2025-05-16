#include "apikey.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("API key getter and setter") {
    Api api;

    SECTION("Initially key is empty") {
        REQUIRE(api.getKey() == "");
    }

    SECTION("Setting and getting key") {
        api.setKey("NASA1234");
        REQUIRE(api.getKey() == "NASA1234");
    }

    SECTION("Overwriting key") {
        api.setKey("OLDKEY");
        api.setKey("NEWKEY");
        REQUIRE(api.getKey() == "NEWKEY");
    }
}
#include "event.hpp"
#include <nlohmann/json.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Event constructor and getters", "[Event]") {
    Event e("Wildfire", "2024-06-01", 45.0, -122.0, "Fire");

    REQUIRE(e.getName() == "Wildfire");
    REQUIRE(e.getDate() == "2024-06-01");
    REQUIRE(e.getLatitude() == Approx(45.0));
    REQUIRE(e.getLongitude() == Approx(-122.0));
    REQUIRE(e.getCategory() == "Fire");
}

TEST_CASE("Event setters and state changes", "[Event]") {
    Event e("Initial", "2024-01-01", 0.0, 0.0, "None");

    e.setName("Updated");
    e.setDate("2024-12-31");
    e.setLatitude(10.5);
    e.setLongitude(-20.5);
    e.setCategory("Flood");

    REQUIRE(e.getName() == "Updated");
    REQUIRE(e.getDate() == "2024-12-31");
    REQUIRE(e.getLatitude() == Approx(10.5));
    REQUIRE(e.getLongitude() == Approx(-20.5));
    REQUIRE(e.getCategory() == "Flood");
}

TEST_CASE("Event edge cases", "[Event]") {
    Event e("", "", -90.0, 180.0, "");

    REQUIRE(e.getName().empty());
    REQUIRE(e.getDate().empty());
    REQUIRE(e.getLatitude() == Approx(-90.0));
    REQUIRE(e.getLongitude() == Approx(180.0));
    REQUIRE(e.getCategory().empty());
}

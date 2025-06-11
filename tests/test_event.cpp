#include "event.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

TEST_CASE("Default constructor creates empty event") {
    Event e;
    REQUIRE(e.getId().empty());
    REQUIRE(e.getTitle().empty());
    REQUIRE(e.getDescription() == std::nullopt);
    REQUIRE(e.getPos().first == Catch::Approx(0.0));
    REQUIRE(e.getPos().second == Catch::Approx(0.0));
}

TEST_CASE("JSON constructor sets fields correctly") {
    json j = {
        {"id", "abc123"},
        {"title", "Volcano"},
        {"description", "Lava eruption"},
        {"geometry", {{{"coordinates", {12.3, 45.6}}}}}
    };
    Event e(j);

    REQUIRE(e.getId() == "abc123");
    REQUIRE(e.getTitle() == "Volcano");
    REQUIRE(e.getDescription().has_value());
    REQUIRE(e.getDescription().value() == "Lava eruption");

    auto [lon, lat] = e.getPos();
    REQUIRE(lon == Catch::Approx(12.3));
    REQUIRE(lat == Catch::Approx(45.6));
}

TEST_CASE("Setters update event fields") {
    Event e;
    e.setId("id987");
    e.setTitle("Storm");
    e.setDescription("Severe wind");
    e.setPos(100.5, 50.25);

    REQUIRE(e.getId() == "id987");
    REQUIRE(e.getTitle() == "Storm");
    REQUIRE(e.getDescription().has_value());
    REQUIRE(e.getDescription().value() == "Severe wind");

    auto [lon, lat] = e.getPos();
    REQUIRE(lon == Catch::Approx(100.5));
    REQUIRE(lat == Catch::Approx(50.25));
}

TEST_CASE("Default link and closed fields are empty") {
    Event e;
    REQUIRE(e.getLink().empty());
    REQUIRE(!e.getClosed().has_value());
}

TEST_CASE("JSON constructor handles all optional fields, categories, sources, and geometry types") {
    json j = {
        {"id", "evt1"},
        {"title", "Event Title"},
        {"description", "Desc"},
        {"link", "http://example.com"},
        {"closed", "2025-06-01"},
        {"categories", {
            {{"id","cat1"},{"title","Category 1"}},
            {{"id","cat2"},{"title","Category 2"}}
        }},
        {"sources", {
            {{"id","src1"},{"url","http://src1.com"}},
            {{"id","src2"},{"url", nullptr}}
        }},
        {"geometry", {
            {
                {"date","2025-06-05T00:00:00Z"},
                {"type","Point"},
                {"coordinates",{10.0,20.0}}
            },
            {
                {"date","2025-06-06T00:00:00Z"},
                {"type","MultiPoint"},
                {"coordinates",{{30.0,40.0},{50.0,60.0}}},
                {"magnitudeUnit","mU"},
                {"magnitudeValue",7.7}
            },
            {
                {"date","2025-06-07T00:00:00Z"},
                {"type","Polygon"},
                {"coordinates",{{{70.0,80.0},{90.0,100.0},{70.0,80.0}}}}
            }
        }}
    };

    Event e(j);
    REQUIRE(e.getId() == "evt1");
    REQUIRE(e.getTitle() == "Event Title");
    REQUIRE(e.getDescription().has_value());
    REQUIRE(e.getDescription().value() == "Desc");
    REQUIRE(e.getLink() == "http://example.com");
    REQUIRE(e.getClosed().has_value());
    REQUIRE(e.getClosed().value() == "2025-06-01");

    auto cats = e.getCategories();
    REQUIRE(cats.size() == 2);
    REQUIRE(cats[0].id == "cat1");
    REQUIRE(cats[0].title == "Category 1");
    REQUIRE(cats[1].id == "cat2");
    REQUIRE(cats[1].title == "Category 2");

    auto srcs = e.getSources();
    REQUIRE(srcs.size() == 2);
    REQUIRE(srcs[0].id == "src1");
    REQUIRE(srcs[0].url.has_value());
    REQUIRE(srcs[0].url.value() == "http://src1.com");
    REQUIRE(srcs[1].id == "src2");
    REQUIRE(!srcs[1].url.has_value());

    auto geoms = e.getGeometry();
    REQUIRE(geoms.size() == 3);

    // Point geometry
    REQUIRE(geoms[0].date == "2025-06-05T00:00:00Z");
    REQUIRE(geoms[0].type == "Point");
    REQUIRE(geoms[0].coordinates.size() == 2);
    REQUIRE(geoms[0].coordinates[0] == Catch::Approx(10.0));
    REQUIRE(geoms[0].coordinates[1] == Catch::Approx(20.0));
    REQUIRE(!geoms[0].coordinatesList.has_value());
    REQUIRE(!geoms[0].magnitudeUnit.has_value());
    REQUIRE(!geoms[0].magnitudeValue.has_value());

    // MultiPoint geometry
    REQUIRE(geoms[1].date == "2025-06-06T00:00:00Z");
    REQUIRE(geoms[1].type == "MultiPoint");
    REQUIRE(geoms[1].coordinates.size() == 2);
    REQUIRE(geoms[1].coordinates[0] == Catch::Approx(30.0));
    REQUIRE(geoms[1].coordinates[1] == Catch::Approx(40.0));
    REQUIRE(geoms[1].coordinatesList.has_value());
    REQUIRE(geoms[1].coordinatesList->size() == 2);
    REQUIRE((*geoms[1].coordinatesList)[1][0] == Catch::Approx(50.0));
    REQUIRE((*geoms[1].coordinatesList)[1][1] == Catch::Approx(60.0));
    REQUIRE(geoms[1].magnitudeUnit.has_value());
    REQUIRE(geoms[1].magnitudeUnit.value() == "mU");
    REQUIRE(geoms[1].magnitudeValue.has_value());
    REQUIRE(geoms[1].magnitudeValue.value() == Catch::Approx(7.7));

    // Polygon geometry
    REQUIRE(geoms[2].date == "2025-06-07T00:00:00Z");
    REQUIRE(geoms[2].type == "Polygon");
    REQUIRE(geoms[2].coordinatesList.has_value());
    REQUIRE(geoms[2].coordinatesList->size() == 3);
    REQUIRE(geoms[2].coordinates[0] == (*geoms[2].coordinatesList)[0][0]);
}

TEST_CASE("toJson produces equivalent JSON for round-trip") {
    json j = {
        {"id","roundtrip"},
        {"title","Round Trip"},
        {"description","Test"},
        {"link","link"},
        {"closed","close"},
        {"categories", {{{"id","x"},{"title","X"}}}},
        {"sources", {{{"id","y"},{"url","urlY"}}}},
        {"geometry", {{{"date","dateY"},{"type","T"},{"coordinates",{5.5,6.6}}}}}
    };
    Event e(j);
    json j2 = e.toJson();
    REQUIRE(j2["id"] == j["id"]);
    REQUIRE(j2["title"] == j["title"]);
    REQUIRE(j2["description"] == std::string(j["description"]));
    REQUIRE(j2["link"] == j["link"]);
    REQUIRE(j2["closed"] == std::string(j["closed"]));
    REQUIRE(j2["categories"].size() == 1);
    REQUIRE(j2["sources"].size() == 1);
    REQUIRE(j2["geometry"].size() == 1);
}
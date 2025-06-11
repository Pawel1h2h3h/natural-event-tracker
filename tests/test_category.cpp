

#include <catch2/catch_test_macros.hpp>
#include <nlohmann/json.hpp>
#include <sstream>
#include <iostream>
#include "category.hpp"

using json = nlohmann::json;

TEST_CASE("Default constructor yields empty fields") {
    Category c;
    REQUIRE(c.getId().empty());
    REQUIRE(c.getTitle().empty());
    REQUIRE(c.getDescription().empty());
    REQUIRE(c.getLink().empty());
    REQUIRE(c.getLayers().empty());
}

TEST_CASE("Constructor sets fields correctly") {
    Category c("id1", "Title1", "Desc1", "http://link", "layerA");
    REQUIRE(c.getId() == "id1");
    REQUIRE(c.getTitle() == "Title1");
    REQUIRE(c.getDescription() == "Desc1");
    REQUIRE(c.getLink() == "http://link");
    REQUIRE(c.getLayers() == "layerA");
}

TEST_CASE("fromJson constructs correctly with all keys") {
    json j = {
        {"id", "catX"},
        {"title", "Cat Title"},
        {"description", "Some description"},
        {"link", "http://example.com"},
        {"layers", "L1,L2"}
    };
    Category c = Category::fromJson(j);
    REQUIRE(c.getId() == "catX");
    REQUIRE(c.getTitle() == "Cat Title");
    REQUIRE(c.getDescription() == "Some description");
    REQUIRE(c.getLink() == "http://example.com");
    REQUIRE(c.getLayers() == "L1,L2");
}

TEST_CASE("fromJson defaults missing keys to empty") {
    json j = {
        {"id", "onlyId"}
        // no other keys
    };
    Category c = Category::fromJson(j);
    REQUIRE(c.getId() == "onlyId");
    REQUIRE(c.getTitle().empty());
    REQUIRE(c.getDescription().empty());
    REQUIRE(c.getLink().empty());
    REQUIRE(c.getLayers().empty());
}

TEST_CASE("toJson produces correct JSON") {
    Category c("id2", "Title2", "Desc2", "link2", "layers2");
    json j = c.toJson();
    REQUIRE(j.is_object());
    REQUIRE(j["id"] == "id2");
    REQUIRE(j["title"] == "Title2");
    REQUIRE(j["description"] == "Desc2");
    REQUIRE(j["link"] == "link2");
    REQUIRE(j["layers"] == "layers2");
}

TEST_CASE("print outputs expected format") {
    Category c("id3", "My Title", "LineDesc", "http://l", "ly");
    std::ostringstream oss;
    auto* old_buf = std::cout.rdbuf(oss.rdbuf());
    c.print();
    std::cout.rdbuf(old_buf);
    std::string out = oss.str();
    std::string expected;
    expected += "[id3] My Title\n";
    expected += "LineDesc\n";
    expected += "link:   http://l\n";
    expected += "layers: ly\n\n";
    REQUIRE(out == expected);
}
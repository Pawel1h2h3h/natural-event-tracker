

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <string>
#include <nlohmann/json.hpp>
#include "fileHandler.hpp"
#include "event.hpp"
#include "category.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// Helper to create a unique temporary directory
static fs::path createTempDir() {
    auto path = fs::temp_directory_path() / ("fh_test_" + std::to_string(std::time(nullptr)));
    fs::create_directories(path);
    return path;
}

// RAII guard to restore current_path after each test
struct CwdGuard {
    fs::path oldcwd;
    CwdGuard() : oldcwd(fs::current_path()) {}
    ~CwdGuard() { fs::current_path(oldcwd); }
};

TEST_CASE("setJData and getJData store JSON correctly") {
    FileHandler fh;
    json j = {{"a", 1}, {"b", 2}};
    fh.setJData(j);
    REQUIRE(fh.getJData() == j);
}

TEST_CASE("readFromJson throws when data directory missing") {
    CwdGuard guard;
    auto tmp = createTempDir();
    fs::create_directory(tmp / "cwd");
    fs::current_path(tmp / "cwd");
    FileHandler fh;
    REQUIRE_THROWS_WITH(
        fh.readFromJson("any.json"),
        Catch::Matchers::ContainsSubstring("data directory does not exist")
    );
}

TEST_CASE("readFromJson throws when file cannot be opened") {
    CwdGuard guard;
    auto tmp = createTempDir();
    fs::create_directories(tmp / "data");
    fs::create_directory(tmp / "cwd");
    fs::current_path(tmp / "cwd");
    FileHandler fh;
    REQUIRE_THROWS_WITH(
        fh.readFromJson("nofile.json"),
        Catch::Matchers::ContainsSubstring("cannot open file")
    );
}

TEST_CASE("readFromJson throws on invalid JSON") {
    CwdGuard guard;
    auto tmp = createTempDir();
    fs::create_directories(tmp / "data");
    fs::create_directory(tmp / "cwd");
    fs::current_path(tmp / "cwd");
    // Create invalid JSON file
    auto badFile = tmp / "data" / "bad.json";
    std::ofstream(badFile) << "not valid json";
    FileHandler fh;
    REQUIRE_THROWS_WITH(
        fh.readFromJson("bad.json"),
        Catch::Matchers::ContainsSubstring("JSON parsing error")
    );
}

TEST_CASE("readFromJson reads and sets j_data successfully") {
    CwdGuard guard;
    auto tmp = createTempDir();
    fs::create_directories(tmp / "data");
    fs::create_directory(tmp / "cwd");
    fs::current_path(tmp / "cwd");
    // Create valid JSON file
    auto goodFile = tmp / "data" / "good.json";
    json j = {{"key", "value"}};
    std::ofstream(goodFile) << j.dump();
    FileHandler fh;
    REQUIRE(fh.readFromJson("good.json"));
    REQUIRE(fh.getJData() == j);
}

TEST_CASE("createEvents returns vector of Events on valid format") {
    FileHandler fh;
    // JSON format: [ <ignored>, [ event1, event2 ] ]
    json j = json::array();
    j.push_back(nullptr);
    json arr = json::array();
    arr.push_back({{"id", "e1"}});
    arr.push_back({{"id", "e2"}});
    j.push_back(arr);
    fh.setJData(j);
    auto events = fh.createEvents();
    REQUIRE(events.size() == 2);
    REQUIRE(events[0].getId() == "e1");
    REQUIRE(events[1].getId() == "e2");
}

TEST_CASE("createEvents throws on invalid format") {
    FileHandler fh;
    fh.setJData(json::object());
    REQUIRE_THROWS_WITH(
        fh.createEvents(),
        Catch::Matchers::ContainsSubstring("invalid JSON file format for creating Events")
    );
}

TEST_CASE("createRecentEvents modifies date for events with geometry") {
    FileHandler fh;
    json j;
    j["events"] = json::array();
    // Event with two geometry entries
    j["events"].push_back({
        {"id", "e1"},
        {"geometry", json::array({
            {{"date", "d1"}, {"type", "T"}, {"coordinates", {1.0, 2.0}}},
            {{"date", "d2"}, {"type", "T"}, {"coordinates", {3.0, 4.0}}}
        })}
    });
    // Event without geometry
    j["events"].push_back({{"id", "e2"}});
    fh.setJData(j);
    auto recents = fh.createRecentEvents();
    REQUIRE(recents.size() == 2);
    // First event: geometry[0].date should be updated to last date "d2"
    auto geoms1 = recents[0].getGeometry();
    REQUIRE(geoms1.size() == 2);
    REQUIRE(geoms1[0].date == "d2");
    // Second event: no geometry
    auto geoms2 = recents[1].getGeometry();
    REQUIRE(geoms2.empty());
}

TEST_CASE("createRecentEvents throws on invalid format") {
    FileHandler fh;
    fh.setJData(json::array());
    REQUIRE_THROWS_WITH(
        fh.createRecentEvents(),
        Catch::Matchers::ContainsSubstring("invalid JSON file format for creating Events")
    );
}

TEST_CASE("createCategories returns vector of Category objects") {
    FileHandler fh;
    json cat0 = {{"id", "c1"}, {"title", "T1"}};
    json cat1 = {{"id", "c2"}, {"title", "T2"}};
    json j = json::array({ json::array({cat0, cat1}) });
    fh.setJData(j);
    auto cats = fh.createCategories();
    REQUIRE(cats.size() == 2);
    REQUIRE(cats[0].getId() == "c1");
    REQUIRE(cats[0].getTitle() == "T1");
    REQUIRE(cats[1].getId() == "c2");
    REQUIRE(cats[1].getTitle() == "T2");
}

TEST_CASE("writeToJson writes data correctly") {
    CwdGuard guard;
    auto tmp = createTempDir();
    fs::create_directory(tmp / "cwd");
    fs::current_path(tmp / "cwd");
    FileHandler fh;
    json j = {{"a", 1}, {"b", 2}};
    fh.setJData(j);
    REQUIRE(fh.writeToJson("out.json"));
    fs::path outpath = tmp / "data" / "out.json";
    REQUIRE(fs::exists(outpath));
    // Read back and verify array of values [1,2]
    std::ifstream in(outpath);
    json j2;
    in >> j2;
    REQUIRE(j2.is_array());
    REQUIRE(j2.size() == 2);
    REQUIRE(j2[0] == 1);
    REQUIRE(j2[1] == 2);
}

TEST_CASE("doCategoriesExist returns true when categories.json exists") {
    CwdGuard guard;
    auto tmp = createTempDir();
    fs::create_directories(tmp / "data");
    fs::create_directory(tmp / "cwd");
    fs::current_path(tmp / "cwd");
    std::ofstream(tmp / "data" / "categories.json");
    FileHandler fh;
    REQUIRE(fh.doCategoriesExist());
}



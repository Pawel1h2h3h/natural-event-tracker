#include <catch2/catch_test_macros.hpp>
#include "apikey.hpp"
#include <nlohmann/json.hpp>
#include <map>
#include <cpr/cpr.h>

using json = nlohmann::json;
using Params = cpr::Parameters;
using Header = cpr::Header;

// Helper to convert cpr::Parameters or Header to std::map for easy inspection
static std::map<std::string, std::string> to_map(const cpr::Parameters& c) {
    // Hack to expose the underlying containerList_
    struct Hack : public cpr::Parameters {
        using cpr::Parameters::containerList_;
    };
    const auto& list = reinterpret_cast<const Hack&>(c).containerList_;
    std::map<std::string, std::string> m;
    for (const auto& p : list) {
        m[p.key] = p.value;
    }
    return m;
}

static std::map<std::string, std::string> to_map(const cpr::Header& c) {
    // cpr::Header is an alias for std::map with case-insensitive keys
    return std::map<std::string, std::string>(c.begin(), c.end());
}

TEST_CASE("setApiKey sets key and Authorization header") {
    Api api;
    api.setApiKey("MYKEY");
    const auto& req = api.getCurrentRequest();
    auto headers = to_map(req.headers);
    REQUIRE(headers["Authorization"] == "Bearer MYKEY");
}

TEST_CASE("setLimit adds limit param") {
    Api api;
    api.setLimit(42);
    const auto& req = api.getCurrentRequest();
    auto params = to_map(req.params);
    REQUIRE(params["limit"] == "42");
}

TEST_CASE("setCategory sets URL and status/category params") {
    Api api;
    api.setCategory("cat123");
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/events");
    auto params = to_map(req.params);
    REQUIRE(params["status"] == "open");
    REQUIRE(params["category"] == "cat123");
}

TEST_CASE("requestEvents sets URL and status param") {
    Api api;
    api.requestEvents("closed");
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/events");
    auto params = to_map(req.params);
    REQUIRE(params["status"] == "closed");
}

TEST_CASE("requestEventsDays sets URL and days param") {
    Api api;
    api.requestEventsDays(7);
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/events");
    auto params = to_map(req.params);
    REQUIRE(params["days"] == "7");
}

TEST_CASE("requestCategoryById sets URL and status param") {
    Api api;
    std::string status = "open";
    api.requestCategoryById("catX", status);
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/categories/catX");
    auto params = to_map(req.params);
    REQUIRE(params["status"] == "open");
}

TEST_CASE("requestSources sets correct URL with no params") {
    Api api;
    api.requestSources();
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/sources");
    REQUIRE(to_map(req.params).empty());
}

TEST_CASE("requestMagnitudes sets correct URL with no params") {
    Api api;
    api.requestMagnitudes();
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/magnitudes");
    REQUIRE(to_map(req.params).empty());
}

TEST_CASE("requestCategories sets correct URL with no params") {
    Api api;
    api.requestCategories();
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/categories");
    REQUIRE(to_map(req.params).empty());
}

TEST_CASE("requestSingleEvent sets correct URL with no params") {
    Api api;
    api.requestSingleEvent("evt42");
    const auto& req = api.getCurrentRequest();
    REQUIRE(req.url.str() == "https://eonet.gsfc.nasa.gov/api/v3/events/evt42");
    REQUIRE(to_map(req.params).empty());
}

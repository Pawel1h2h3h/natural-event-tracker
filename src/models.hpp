#pragma once
#include <string>
#include <vector>
#include <optional>
#include <cpr/cpr.h>

struct ApiRequest {
    cpr::Url url;
    cpr::Parameters params;
    cpr::Header headers;
};


struct CategoryS {
    std::string id;
    std::string title;
};

struct Source {
    std::string id;
    std::optional<std::string> url;
};

struct Geometry {
    std::string date;
    std::string type;
    std::vector<double> coordinates;
    std::optional<std::string> magnitudeUnit;
    std::optional<double> magnitudeValue;
};
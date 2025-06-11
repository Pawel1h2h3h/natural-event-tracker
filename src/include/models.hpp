#pragma once
#include <string>
#include <vector>
#include <optional>
#include <cpr/cpr.h>

/**
 * @brief Represents an API request with URL, parameters, and headers.
 */
struct ApiRequest {
    cpr::Url url;               ///< The URL of the API endpoint.
    cpr::Parameters params;     ///< The parameters to be sent with the request.
    cpr::Header headers;        ///< The headers to be included in the request.
};


/**
 * @brief Represents a category with an ID and title.
 */
struct CategoryS {
    std::string id;     ///< The unique identifier of the category.
    std::string title;  ///< The title or name of the category.
};

/**
 * @brief Represents a source with an ID and an optional URL.
 */
struct Source {
    std::string id;                         ///< The unique identifier of the source.
    std::optional<std::string> url;        ///< The optional URL associated with the source.
};

/**
 * @brief Represents geometric data including date, type, coordinates, and optional magnitude.
 */
struct Geometry {
    std::string date;                                      ///< The date associated with the geometry.
    std::string type;                                      ///< The type of geometry (e.g., Point, Polygon).
    std::vector<double> coordinates;                       ///< The coordinates for the geometry.
    std::optional<std::vector<std::vector<double>>> coordinatesList; ///< Optional list of coordinate sets (e.g., for polygons).
    std::optional<std::string> magnitudeUnit;              ///< Optional unit of the magnitude.
    std::optional<double> magnitudeValue;                   ///< Optional value of the magnitude.
};
#include <iostream>
#include <stdexcept>
#include "event.hpp"
using json = nlohmann::json;

/// @class Event
/// @brief Represents a natural event with metadata, categories, sources, and geometry.
///
/// This class parses and stores event data from a JSON object, including optional fields and geometry handling.
/// It supports JSON serialization and printing in human-readable format.
Event::Event(){}

Event::Event(json dict) {
    id = dict.value("id", "");
    title = dict.value("title", "");

    if (dict.contains("description") && !dict["description"].is_null())
        description = dict["description"].get<std::string>();

    link = dict.value("link", "");

    if (dict.contains("closed") && !dict["closed"].is_null())
        closed = dict["closed"].get<std::string>();

    if (dict.contains("categories") && dict["categories"].is_array()) {
        for (const auto& c : dict["categories"]) {
            categories.push_back(CategoryS{
                c.value("id", ""),
                c.value("title", "")
            });
        }
    }

    if (dict.contains("sources") && dict["sources"].is_array()) {
        for (const auto& s : dict["sources"]) {
            Source source;
            source.id = s.value("id", "");
            if (s.contains("url") && !s["url"].is_null())
                source.url = s["url"].get<std::string>();
            sources.push_back(source);
        }
    }

    if (dict.contains("geometry") && dict["geometry"].is_array()) {
        for (const auto& g : dict["geometry"]) {
            Geometry geo;
            geo.date = g.value("date", "");
            geo.type = g.value("type", "");
            try {
                const auto& coords = g["coordinates"];
                if (coords.is_array()) {
                    if (coords.size() >= 2 && coords[0].is_number()) {
                        // [lon, lat]
                        geo.coordinates = coords.get<std::vector<double>>();
                    } else if (coords[0].is_array() && !coords[0].empty()) {
                        if (coords[0][0].is_number()) {
                            // [[lon, lat], [lon, lat], ...]
                            std::vector<std::vector<double>> extracted;
                            for (const auto& point : coords) {
                                if (point.is_array() && point.size() >= 2) {
                                    extracted.push_back(point.get<std::vector<double>>());
                                }
                            }
                            if (!extracted.empty()) {
                                geo.coordinatesList = extracted;
                                geo.coordinates = extracted.front();
                            }
                        } else if (coords[0][0].is_array()) {
                            // [[[lon, lat], ...]] — Polygon
                            const auto& ring = coords[0];
                            std::vector<std::vector<double>> extracted;
                            for (const auto& point : ring) {
                                if (point.is_array() && point.size() >= 2) {
                                    extracted.push_back(point.get<std::vector<double>>());
                                }
                            }
                            if (!extracted.empty()) {
                                geo.coordinatesList = extracted;
                                geo.coordinates = extracted.front();
                            }
                        }
                    }
                }
            } catch (const std::exception& e) {
                throw std::runtime_error(std::string("Error while processing geometry.coordinates: ") + e.what());
            } catch (...) {
                throw std::runtime_error("Unknown error while processing geometry.coordinates");
            }
            if (g.contains("magnitudeUnit") && !g["magnitudeUnit"].is_null())
                geo.magnitudeUnit = g["magnitudeUnit"].get<std::string>();
            if (g.contains("magnitudeValue") && !g["magnitudeValue"].is_null())
                geo.magnitudeValue = g["magnitudeValue"].get<double>();
            geometry.push_back(geo);
        }
    }
}

std::string Event::getId() const {return id;}


// New setter methods
void Event::setId(const std::string& new_id) { id = new_id; }
void Event::setTitle(const std::string& new_title) { title = new_title; }
void Event::setDescription(const std::optional<std::string>& new_description) { description = new_description; }
void Event::setLink(const std::string& new_link) { link = new_link; }
void Event::setClosed(const std::optional<std::string>& new_closed) { closed = new_closed; }

// New getter methods
const std::string& Event::getTitle() const { return title; }
const std::optional<std::string>& Event::getDescription() const { return description; }
const std::string& Event::getLink() const { return link; }
const std::optional<std::string>& Event::getClosed() const { return closed; }
const std::vector<CategoryS>& Event::getCategories() const { return categories; }
const std::vector<Source>& Event::getSources() const { return sources; }
const std::vector<Geometry>& Event::getGeometry() const { return geometry; }


json Event::toJson() const {
    json j_event;
    j_event["id"] = id;
    j_event["title"] = title;
    j_event["description"] = description.value_or("");
    j_event["link"] = link;
    j_event["closed"] = closed.value_or("");

    j_event["categories"] = json::array();
    for (const auto& c : categories) {
        j_event["categories"].push_back({
            {"id", c.id},
            {"title", c.title}
        });
    }

    j_event["sources"] = json::array();
    for (const auto& s : sources) {
        j_event["sources"].push_back({
            {"id", s.id},
            {"url", s.url}
        });
    }

    j_event["geometry"] = json::array();
    for (const auto& g : geometry) {
        json geo = {
            {"date", g.date},
            {"type", g.type},
            {"coordinates", g.coordinates}
        };
        if (g.coordinatesList.has_value()) {
            geo["coordinatesList"] = g.coordinatesList.value();
        }
        if (g.magnitudeUnit.has_value()) {
            geo["magnitudeUnit"] = g.magnitudeUnit.value();
        }
        if (g.magnitudeValue.has_value()) {
            geo["magnitudeValue"] = g.magnitudeValue.value();
        }
        j_event["geometry"].push_back(geo);
    }

    return j_event;
}

void Event::print_json_repr() const {
    std::cout << toJson().dump(4);
}

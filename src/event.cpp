#include <iostream>
#include "event.hpp"
using json = nlohmann::json;

Event::Event(){}

Event::Event(json dict) {
    id = dict.at("id").get<std::string>();
    title = dict.at("title").get<std::string>();
    if (dict.contains("description") && !dict["description"].is_null())
        description = dict["description"].get<std::string>();
    if (dict.contains("link"))
        link = dict["link"].get<std::string>();
    if (dict.contains("closed") && !dict["closed"].is_null())
        closed = dict["closed"].get<std::string>();

    if (dict.contains("categories")) {
        for (const auto& c : dict["categories"]) {
            categories.push_back(CategoryS{c.at("id"), c.at("title")});
        }
    }

    if (dict.contains("sources")) {
        for (const auto& s : dict["sources"]) {
            sources.push_back(Source{s.at("id"), s.at("url")});
        }
    }

    if (dict.contains("geometry")) {
        for (const auto& g : dict["geometry"]) {
            Geometry geo;
            geo.date = g.at("date");
            geo.type = g.at("type");
            geo.coordinates = g.at("coordinates").get<std::vector<double>>();
            if (g.contains("magnitudeUnit"))
                geo.magnitudeUnit = g["magnitudeUnit"].get<std::string>();
            if (g.contains("magnitudeValue"))
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



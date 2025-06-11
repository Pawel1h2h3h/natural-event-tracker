#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "models.hpp"

using json = nlohmann::json;

/// @class Event
/// @brief Represents a structured data model for a single natural event, including metadata and spatial data.
///
/// The Event class encapsulates the full structure of a natural event record, such as one returned from
/// a public API (e.g., NASA EONET). It stores identifiers, title, optional description and closure date,
/// a hyperlink, and associated categories, sources, and geometries.
///
/// It supports complete access through getters and setters, and can be constructed directly from a JSON object.
/// It also provides serialization and printable output for debugging or data exporting.
///
/// Example uses include:
/// - Visualizing or filtering natural events in an application,
/// - Converting structured JSON data into usable objects,
/// - Storing and transmitting event state.
class Event {
    protected:
        std::string id;
        std::string title;
        std::optional<std::string> description;
        std::string link = "";
        std::optional<std::string> closed;

        std::vector<CategoryS> categories;
        std::vector<Source> sources;
        std::vector<Geometry> geometry;
    public:
        Event();
        Event(json dict);
        virtual ~Event() = default;

        std::string getName() const;
        std::string getId() const;
        std::pair<double, double> getPos() const;

        void setPos(double lat, double lon);
        void setName(std::string new_name);
        void setId(int new_id);

        // New setter methods
        void setId(const std::string& new_id);
        void setTitle(const std::string& new_title);
        void setDescription(const std::optional<std::string>& new_description);
        void setLink(const std::string& new_link);
        void setClosed(const std::optional<std::string>& new_closed);

        // New getter methods
        const std::string& getTitle() const;
        const std::optional<std::string>& getDescription() const;
        const std::string& getLink() const;
        const std::optional<std::string>& getClosed() const;
        const std::vector<CategoryS>& getCategories() const;
        const std::vector<Source>& getSources() const;
        const std::vector<Geometry>& getGeometry() const;

        virtual void print_json_repr() const;
        virtual json toJson() const;

};
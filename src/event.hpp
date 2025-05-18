#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include "models.hpp"

using json = nlohmann::json;


class Event {
    protected:
        std::string id;
        std::string title;
        std::optional<std::string> description;
        std::string link;
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
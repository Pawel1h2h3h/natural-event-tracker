#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Event {
    private:
        int id;
        std::string name;
        std::pair<double, double> pos;
    public:
        Event();
        Event(int id_, std::string name_, double lat, double lon);
        Event(json dict);

        std::string getName();
        int getId();
        std::pair<double, double> getPos();

        void setPos(double lat, double lon);
        void setName(std::string new_name);
        void setId(int new_id);

        void print_json_repr();
        json toJson();
        void fromJson(json data);

};
#include <iostream>
#include "event.hpp"
using json = nlohmann::json;

Event::Event(){}

Event::Event(int id_, std::string name_, double lat, double lon) {
    id = id_; name = name_; pos = std::make_pair(lat, lon);
}

Event::Event(json dict){
    id = dict["id"]; name = dict["name"]; pos = std::make_pair(dict["latitude"], dict["longitude"]);
}

int Event::getId(){return id;}

std::string Event::getName(){return name;}

std::pair<double, double> Event::getPos(){return pos;}

void Event::setId(int new_id){id=new_id;}

void Event::setName(std::string new_name){name=new_name;}

void Event::setPos(double lat, double lon){
    pos = std::make_pair(lat, lon);
}

json Event::toJson(){
            json j_event = {
            {"id", getId()},
            {"name", getName()},
            {"latitude", getPos().first},
            {"longitude", getPos().second}
        };
    return j_event;
}

void Event::print_json_repr(){
    std::cout << toJson().dump(4);
}

void Event::fromJson(json data){
    Event(data["id"], data["name"], data["latitude"], data["longitude"]);
}

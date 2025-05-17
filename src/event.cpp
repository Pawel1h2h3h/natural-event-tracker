#include <iostream>
#include "event.hpp"

Event::Event(){}

Event::Event(int id_, std::string name_, double lat, double lon) {
    id = id_; name = name_; pos = std::make_pair(lat, lon);
}

int Event::getId(){return id;}

std::string Event::getName(){return name;}

std::pair<double, double> Event::getPos(){return pos;}

void Event::setId(int new_id){id=new_id;}

void Event::setName(std::string new_name){name=new_name;}

void Event::setPos(double lat, double lon){
    pos = std::make_pair(lat, lon);
}

void Event::info(){
    std::cout << id << "\n" << name << "\n";
    std::cout << pos.first << ", " << pos.second;
}

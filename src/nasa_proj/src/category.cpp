// category.cpp
#include "category.hpp"
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>


Category::Category(const std::string& id,
                   const std::string& title,
                   const std::string& description,
                   const std::string& link,
                   const std::string& layers)
    : id(id), title(title), description(description), link(link), layers(layers) {}

Category Category::fromJson(const nlohmann::json& j) {
    return Category(
        j.value("id", ""),
        j.value("title", ""),
        j.value("description", ""),
        j.value("link", ""),
        j.value("layers", "")
    );
}

nlohmann::json Category::toJson() const {
    return {
        {"id", id},
        {"title", title},
        {"description", description},
        {"link", link},
        {"layers", layers}
    };
}

void Category::print() const {
    std::ostringstream oss;
    oss << "[" << id << "] " << title << "\n"
        << description << "\n"
        << "link:   " << link << "\n"
        << "layers: " << layers << "\n\n";
    std::cout << oss.str();
}

const std::string& Category::getId() const { return id; }
const std::string& Category::getTitle() const { return title; }
const std::string& Category::getDescription() const { return description; }
const std::string& Category::getLink() const { return link; }
const std::string& Category::getLayers() const { return layers; }

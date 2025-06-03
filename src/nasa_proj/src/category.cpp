// category.cpp
#include "category.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

Category::Category(const std::string& id,
                   const std::string& title,
                   const std::string& description,
                   const std::string& link,
                   const std::string& layers)
    : id(id), title(title), description(description), link(link), layers(layers) {}

Category Category::fromJson(const nlohmann::json& j) {
    return Category(
        j.at("id"),
        j.at("title"),
        j.at("description"),
        j.at("link"),
        j.at("layers")
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
    std::cout << "[" << id << "] " << title << "\n";
    std::cout << description << "\n";
    std::cout << "link:   " << link << "\n";
    std::cout << "layers: " << layers << "\n\n";
}

const std::string& Category::getId() const { return id; }
const std::string& Category::getTitle() const { return title; }
const std::string& Category::getDescription() const { return description; }
const std::string& Category::getLink() const { return link; }
const std::string& Category::getLayers() const { return layers; }

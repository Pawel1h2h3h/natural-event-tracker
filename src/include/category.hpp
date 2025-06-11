// category.hpp
#pragma once
#include <string>
#include <nlohmann/json.hpp>



/**
 * @class Category
 * @brief Represents a category with an ID, title, description, link, and layers.
 *
 * This class provides methods to construct a category object from JSON data,
 * convert it back to JSON, and print its details.
 */
class Category {
private:
    std::string id;
    std::string title;
    std::string description;
    std::string link;
    std::string layers;

public:
    Category() = default;
    Category(const std::string& id,
             const std::string& title,
             const std::string& description,
             const std::string& link,
             const std::string& layers);

    static Category fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;

    void print() const;

    const std::string& getId() const;
    const std::string& getTitle() const;
    const std::string& getDescription() const;
    const std::string& getLink() const;
    const std::string& getLayers() const;
};
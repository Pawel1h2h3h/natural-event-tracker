#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "event.hpp"
#include "category.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

/// @class FileHandler
/// @brief Handles reading, writing, and managing JSON-based event and category data.
///
/// This class is responsible for operations on JSON files such as reading from and writing to disk,
/// parsing structured data for events and categories, and managing file system directories relevant to the data.
/// It provides a consistent interface for creating, checking, and clearing data used in the application.
class FileHandler {
    private:
        std::string path_read;
        std::string path_write;
        json j_data;
    public:

        /// Returns the stored JSON data.
        const json& getJData() const;

        /// Writes JSON data to a file in the output directory.
        bool writeToJson(const std::string& path);

        /// Reads JSON data from a file in the data directory.
        bool readFromJson(const std::string& path);

        /// Checks if categories exist.
        bool doCategoriesExist();

        /// Creates the data folder if it doesn't exist.
        std::filesystem::path createFolder();

        /// Sets the JSON data.
        void setJData(const json& new_jdata);

        /// Clears the data directory.
        void clearDataDir();


        /// Returns the parent path.
        fs::path getParentPath();

        /// Returns the path to the data directory.
        fs::path getDataPath();

        /// Creates a vector of Event objects.
        std::vector<Event> createEvents();

        /// Creates a vector of recent Event objects.
        std::vector<Event> createRecentEvents();

        /// Creates a vector of Category objects.
        std::vector<Category> createCategories();



};
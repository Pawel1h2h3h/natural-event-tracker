#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "fileHandler.hpp"
#include "event.hpp"
#include "category.hpp"
#include <vector>

namespace fs = std::filesystem;


/// Sets the internal JSON data to the provided JSON object.
/// @param new_jdata The new JSON data to set.
void FileHandler::setJData(const json& new_jdata){j_data = new_jdata;}

/// Returns a constant reference to the internal JSON data.
/// @return The stored JSON data.
const json& FileHandler::getJData() const {return j_data;}


/// Reads JSON data from a file located in the "data" directory relative to the parent of the current path.
/// Throws an exception if the data directory or file does not exist or if parsing fails.
/// @param filename The name of the JSON file to read.
/// @return True if reading and parsing were successful.
bool FileHandler::readFromJson(const std::string& filename)
{
    fs::path directory_path = fs::current_path().parent_path() / "data";
    if (!fs::exists(directory_path)){
        throw std::runtime_error("Error: data directory does not exist\n");
    }
    auto filepath = directory_path / filename;
    std::ifstream file(filepath);
    if (!file.is_open()){
        throw std::runtime_error("Error: cannot open file\n");
    }
    json j;
    try {
        file >> j;
    } catch (const std::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
    setJData(j);
    return true;
}


/// Creates a vector of Event objects from the stored JSON data.
/// Expects the JSON data to be an array where the second element is an array of events.
/// Throws an exception if the format is invalid.
/// @return A vector of Event objects.
std::vector<Event> FileHandler::createEvents() {
    std::vector<Event> events;

    if (j_data.is_array() && j_data.size() > 1 && j_data[1].is_array()) {
        // format from allEvents.json
        for (const auto& item : j_data[1]) {
            events.emplace_back(item);
        }
    }
     else {
        throw std::runtime_error("Error: invalid JSON file format for creating Events");
    }

    return events;
}


/// Creates a vector of recent Event objects from the stored JSON data.
/// Expects the JSON data to contain an "events" array.
/// For each event, replaces the base JSON's "date" with the last geometry date if geometry exists.
/// Throws an exception if the format is invalid.
/// @return A vector of recent Event objects.
std::vector<Event> FileHandler::createRecentEvents() {
    std::vector<Event> recentEvents;

    if (j_data.contains("events") && j_data["events"].is_array()) {
        for (const auto& item : j_data["events"]) {
            Event evt(item);

            const auto& geometryList = evt.getGeometry();
            if (!geometryList.empty()) {
                // Replace the base JSON's "date" with the last geometry date
                nlohmann::json modified = item;
                modified["geometry"][0]["date"] = geometryList.back().date;
                recentEvents.emplace_back(modified);
            } else {
                recentEvents.push_back(evt);
            }
        }
    } else {
        throw std::runtime_error("Error: invalid JSON file format for creating Events");
    }

    return recentEvents;
}


/// Creates a vector of Category objects from the stored JSON data.
/// Expects the JSON data to be an array where the first element contains category data.
/// @return A vector of Category objects.
std::vector<Category> FileHandler::createCategories() {
    std::vector<Category> categories;
    for (const auto& item : j_data[0]) {
        categories.push_back(Category::fromJson(item));
    }
    return categories;
}


/// Writes the internal JSON data to a JSON file inside a created output folder.
/// The data is written as a JSON array composed of the stored JSON object's values.
/// Throws an exception if the file cannot be opened or writing fails.
/// @param filename The name of the output JSON file.
/// @return True if writing was successful.
bool FileHandler::writeToJson(const std::string& filename)
{
    fs::path filepath = createFolder();       // e.g. "output/"
    filepath /= filename;                     // e.g. "output/file.json"

    fs::create_directories(filepath.parent_path());  // ensure folder exists

    json j_array = json::array();  // JSON array
    for (auto& [key, value] : j_data.items()){
        j_array.push_back(value);
    }

    std::ofstream file(filepath);
    if (!file) {
        throw std::runtime_error("Cannot save file: " + filepath.string() + "\n");
    }

    try {
        file << j_array.dump(4);  // pretty print with indentation
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("JSON write error: ") + e.what() + "\n");
    }

    file.close();

    return true;
}


/// Creates the "data" folder in the parent directory of the current path if it does not exist.
/// @return The path to the "data" folder.
fs::path FileHandler::createFolder()
{
    auto parent_path = fs::current_path().parent_path();
    fs::path target = parent_path / "data";
    fs::create_directory(target);
    return target;

}


/// Returns the parent path of the current working directory.
/// @return The parent path of the current path.
fs::path FileHandler::getParentPath()
{
    auto parent_path = fs::current_path().parent_path();
    return parent_path;
}


/// Returns the path to the "data" folder located in the parent directory of the current path.
/// @return The path to the "data" folder.
fs::path FileHandler::getDataPath()
{
    auto parent_path = fs::current_path().parent_path();
    return parent_path / "data";
}


/// Checks if the "categories.json" file exists inside the "data" folder.
/// @return True if the categories.json file exists, false otherwise.
bool FileHandler::doCategoriesExist() {
    return fs::exists(getParentPath() / "data" / "categories.json");
}


/// Clears all regular files inside the "data" directory.
/// Throws an exception if the folder does not exist or is not a directory,
/// or if any file removal fails.
void FileHandler::clearDataDir() {
    fs::path data_path = getDataPath();
    if (!fs::exists(data_path) || !fs::is_directory(data_path)) {
        throw std::runtime_error("Folder does not exist or is not a directory: " + data_path.string());
    }

    for (const auto& entry : fs::directory_iterator(data_path)) {
        if (fs::is_regular_file(entry.path())) {
            try {
                fs::remove(entry.path());
            } catch (const std::exception& e) {
                throw std::runtime_error("Error deleting file " + entry.path().string() + ": " + e.what());
            }
        }
    }
}
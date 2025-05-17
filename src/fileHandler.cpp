#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "fileHandler.hpp"
#include "event.hpp"
#include "category.hpp"

using namespace std;
namespace fs = std::filesystem;


void FileHandler::setJData(json new_jdata){j_data = new_jdata;}
json FileHandler::getJData(){return j_data;}


bool FileHandler::readFromJson(string filename)
{
    fs::path directory_path = fs::current_path().parent_path() / "data";
    if (!fs::exists(directory_path)){
        cout << "data direcotory not exist";
        return false;
    }
    auto filepath = directory_path / filename;
    ifstream file(filepath);
    if (!file.is_open()){
        cout << "Cannot open file";
        return false;
    }
    json j; file >> j; setJData(j);
    return true;
}

vector<Event> FileHandler::createEvents(){
    vector<Event> events;
    for (auto& [key, value] : j_data.items()) {
        Event event = Event(value);
        events.push_back(event);
}
    return events;
}

std::vector<Category> FileHandler::createCategories() {
    std::vector<Category> categories;
    for (const auto& item : j_data[0]) {
        categories.push_back(Category::fromJson(item));
    }
    return categories;
}

bool FileHandler::writeToJson(std::string filename)
{
    fs::path filepath = createFolder();       // np. "output/"
    filepath /= filename;                     // np. "output/plik.json"

    fs::create_directories(filepath.parent_path());  // upewnij się, że folder istnieje

    json j_array = json::array();  // tablica JSON-owa
    for (auto& [key, value] : j_data.items()){
        j_array.push_back(value);
    }

    ofstream file(filepath);
    if (!file) {
        std::cerr << "Nie można zapisać pliku: " << filepath << "\n";
        return false;
    }

    file << j_array.dump(4);  // pretty print z wcięciem
    file.close();

    return true;
}



fs::path FileHandler::createFolder()
{
    auto parent_path = fs::current_path().parent_path();
    fs::path target = parent_path / "data";
    if(fs::create_directory(target)){
        cout << "Folder utworzony";
    }
    else {cout << "Folder juz istnieje";}
    return target;

}


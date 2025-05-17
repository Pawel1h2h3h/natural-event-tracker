#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include "fileHandler.hpp"
#include "event.hpp"

using namespace std;
namespace fs = std::filesystem;




bool FileHandler::readFromJson(string filename)
{

    return true;
}

bool FileHandler::writeToJson(std::string filename, std::vector<Event> data)
{
    fs::path filepath = createFolder();       // np. "output/"
    filepath /= filename;                     // np. "output/plik.json"

    fs::create_directories(filepath.parent_path());  // upewnij się, że folder istnieje

    json j_array = json::array();  // tablica JSON-owa

    for (auto& event : data) {
        json j_event = {
            {"id", event.getId()},
            {"name", event.getName()},
            {"latitude", event.getPos().first},
            {"longitude", event.getPos().second}
        };
        j_array.push_back(j_event);
    }

    std::ofstream file(filepath);
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


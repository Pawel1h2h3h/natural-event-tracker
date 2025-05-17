#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "event.hpp"

using json = nlohmann::json;
using namespace std;


class FileHandler {
    private:
        string path_read;
        string path_write;
    public:

        bool writeToJson(string path, vector<Event>);
        bool readFromJson(string path);
        filesystem::path createFolder();
};
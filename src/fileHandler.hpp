#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "event.hpp"
#include "category.hpp"

using json = nlohmann::json;
using namespace std;


class FileHandler {
    private:
        string path_read;
        string path_write;
        json j_data;
    public:

        bool writeToJson(string path);
        bool readFromJson(string path);

        filesystem::path createFolder();

        void setJData(json new_jdata);
        json getJData();

        vector<Event> createEvents();

        vector<Category> createCategories();


};
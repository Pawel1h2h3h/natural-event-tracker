#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include "event.hpp"
#include "category.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;


class FileHandler {
    private:
        string path_read;
        string path_write;
        json j_data;
    public:

        bool writeToJson(string path);
        bool readFromJson(string path);

        bool doCategoriesExist();

        filesystem::path createFolder();

        void setJData(json new_jdata);
        json getJData();
        fs::path getParentPath();
        fs::path getDataPath();

        vector<Event> createEvents();

        vector<Category> createCategories();



};
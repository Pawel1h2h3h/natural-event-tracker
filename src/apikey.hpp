#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#pragma once
using namespace std;
using json = nlohmann::json;

class Api {
    private:
        string key;
        string current_url;
    public:
        json getData();

        string getKey();
        string getUrl();
        void setKey(string new_key);
        void setUrl(string url);

};
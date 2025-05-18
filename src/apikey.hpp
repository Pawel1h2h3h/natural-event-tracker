#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "models.hpp"
#include <category.hpp>
#pragma once
using namespace std;
using json = nlohmann::json;


class Api {
private:
    string key;
    ApiRequest current;
public:
    void setCategory(const std::string& category_id);
    void setApiKey(const std::string& api_key);
    void setLimit(int count);
    void requestAllOpenEvents();
    void requestCategoryById(const std::string& category_id);
    void requestLayerById(const std::string& layer_id);
    void requestSources();
    void requestMagnitudes();
    void requestCategories();
    void requestSingleEvent(const std::string& event_id);
    json getData();
};
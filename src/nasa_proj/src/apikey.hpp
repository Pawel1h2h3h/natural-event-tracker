#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "models.hpp"
#include <category.hpp>
#pragma once
using json = nlohmann::json;


class Api {
private:
    std::string key;
    ApiRequest current;
public:
    void setCategory(const std::string& category_id);
    void setApiKey(const std::string& api_key);
    void setLimit(int count);
    void requestEvents(std::string status);
    void requestEventsDays(int days);
    void requestCategoryById(const std::string& category_id, std::string& status);
    void requestSources();
    void requestMagnitudes();
    void requestCategories();
    void requestSingleEvent(const std::string& event_id);
    json getData();

    ApiRequest getCurrentRequest();
};
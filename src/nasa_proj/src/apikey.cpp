#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "apikey.hpp"
#include "models.hpp"
using namespace std;
using json = nlohmann::json;




void Api::setCategory(const std::string& category_id) {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/events"};
    current.params = {{"status", "open"}, {"category", category_id}};
}

void Api::setApiKey(const std::string& api_key) {
    key = api_key;
    current.headers["Authorization"] = "Bearer " + api_key;
}

void Api::setLimit(int count) {
    current.params.Add({"limit", std::to_string(count)});
}

void Api::requestEvents(std::string status) {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/events"};
    current.params = {{"status", status}};
}

void Api::requestEventsDays(int days) {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/events"};
    current.params = {{"days", std::to_string(days)}};
}

json Api::getData() {
    auto r = cpr::Get(current.url, current.params, current.headers);

    if (r.status_code != 200) {
        std::cerr << "Błąd HTTP: " << r.status_code << "\n";
        return json::object();  // pusty JSON
    }

    try {
        return json::parse(r.text);
    } catch (const std::exception& e) {
        std::cerr << "Błąd parsowania JSON: " << e.what() << "\n";
        return json::object();
    }
}




void Api::requestCategoryById(const std::string& category_id, std::string& status) {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/categories/" + category_id};
    current.params = {{"status", status}};
}


void Api::requestSources() {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/sources"};
    current.params = {};
}

void Api::requestMagnitudes() {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/magnitudes"};
    current.params = {};
}

void Api::requestCategories() {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/categories"};
    current.params = {};
}

void Api::requestSingleEvent(const std::string& event_id) {
    current.url = cpr::Url{"https://eonet.gsfc.nasa.gov/api/v3/events/" + event_id};
    current.params = {};
}
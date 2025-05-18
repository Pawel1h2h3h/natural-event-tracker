#include <iostream>
#include "apikey.hpp"
#include "event.hpp"
#include "fileHandler.hpp"
#include <string>
#include "nlohmann/json.hpp"
#include "volcanoEvent.hpp"
using namespace std;


// vector<Category> filescreate(Api& api, FileHandler& fh){
//     // Create event categories
//     fh.readFromJson("categories.json");
//     auto all_cats = fh.createCategories();
//     vector<Category> active_cats;
//     for (auto cat : all_cats) {
//         auto filename = cat.getId();
//         filename.append(".json");

//         api.setUrl(cat.getLink());
//         auto data = api.getData();
//         if(!data["events"].empty()){
//             fh.setJData(data);
//             fh.writeToJson(filename);

//             active_cats.push_back(cat);
//         }
//         else{cout << "Brak danych dla kategorii: " << cat.getId() << "\n";}
//     }
//     return active_cats;
// }

// void createVolcanoes(Api& api, FileHandler& fh){
//     fh.readFromJson("volcanoes.json");
//     nlohmann::json data = fh.getJData();
//     cout << data[1];
    // for (auto& j_vol : fh.getJData()["events"]) {
    //     Event ev(j_vol);
    //     ev.print_json_repr();
    // }

// }


int main() {
    Api api;
    FileHandler fh;
    string key = "H7mCThmUpd7iAwuzs5cMCngPOaDdvLdP4pIEwd4g";
    api.setApiKey(key);

    // 1. Wszystkie aktywne wydarzenia
    api.requestAllOpenEvents();
    api.setLimit(5);
    auto eventsData = api.getData();
    fh.setJData(eventsData);
    fh.writeToJson("allEvents.json");

    // 2. Dane o konkretnej kategorii (np. wildfires)
    api.requestCategoryById("wildfires");
    auto wildfireCat = api.getData();
    fh.setJData(wildfireCat);
    fh.writeToJson("wildfiresCategory.json");

    // 3. Warstwa (layer) dla np. wildfires
    api.requestLayerById("wildfires");
    auto wildfireLayer = api.getData();
    fh.setJData(wildfireLayer);
    fh.writeToJson("wildfiresLayer.json");

    // 4. Źródła danych (sources)
    api.requestSources();
    auto sources = api.getData();
    fh.setJData(sources);
    fh.writeToJson("sources.json");

    // 5. Lista kategorii
    api.requestCategories();
    auto categories = api.getData();
    fh.setJData(categories);
    fh.writeToJson("categories.json");

    // 6. Lista magnitud
    api.requestMagnitudes();
    auto magnitudes = api.getData();
    fh.setJData(magnitudes);
    fh.writeToJson("magnitudes.json");

    // 7. Pojedyncze wydarzenie (np. jedno z `allEvents.json`)
    api.requestSingleEvent("EONET_6543");
    auto singleEvent = api.getData();
    fh.setJData(singleEvent);
    fh.writeToJson("singleEvent.json");

    return 0;
}
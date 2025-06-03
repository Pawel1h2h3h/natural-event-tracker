#include "mapController.hpp"
#include "nasa_proj/src/event.hpp"
#include "nasa_proj/src/category.hpp"
#include "nasa_proj/src/fileHandler.hpp"

#include <QDebug>

MapController::MapController(QObject* parent)
    : QObject(parent)
{}

void MapController::addEvent(const Event& event) {

    QString name = QString::fromStdString(event.getTitle());

    const auto& geometryList = event.getGeometry();
    QGeoCoordinate coord;
    QString date = "N/A";
    if (!geometryList.empty()) {
        auto pos = geometryList[0].coordinates;
        coord = QGeoCoordinate(pos[1], pos[0]);
        date = QString::fromStdString(geometryList[0].date);
    }

    QString type = QString::fromStdString(event.getCategories()[0].id);
    QString description = QString::fromStdString(event.getDescription().value_or(""));
    QString link = QString::fromStdString(event.getLink());

    QString color = "#ff0000";  // default
    if (type == "wildfires") { color = "#ff6600";}
    else if (type == "volcanoes") { color = "#0066ff";}
    else if (type == "seaLakeIce") { color = "#9900cc"; }
    else if (type == "severeStorms") {color = "#d705d7";}

    current_events.push_back(event);
    emit addMarker(name, coord, type, color, date, description, link);


}

void MapController::generateExistingCategories() {
    auto fh = getFh();
    auto api = getApi();

    if (!fh.doCategoriesExist()){
        // 5. Lista kategorii
        api.requestCategories();
        auto categories = api.getData();
        fh.setJData(categories);
        fh.writeToJson("categories.json");
    }


    fh.readFromJson("categories.json");
    auto all_cats = fh.createCategories();
    std::vector<Category> active_cats;

    for (const auto& cat : all_cats) {
        auto filename = cat.getId() + ".json";
        api.requestCategoryById(cat.getId());
        auto data = api.getData();

        if (!data["events"].empty()) {
            fh.setJData(data);
            fh.writeToJson(filename);
            active_cats.push_back(cat);
        } else {
            std::cout << "Brak danych dla kategorii: " << cat.getId() << "\n";
        }
    }
    setCategories(active_cats);
}

void MapController::generateAllEvents() {
    auto fh = getFh();

    for (const auto& cat : getCategories()) {
        auto filename = cat.getId() + ".json";
        fh.readFromJson(filename);

        auto events = fh.createEvents();
        for (const auto& event : events) {
            addEvent(event);
        }
    }
}

void MapController::clearEvents() {
    current_events.clear();       // czyści wektor eventów
    emit clearMarkers();          // sygnalizuje do QML-a, by usunąć markery z mapy
}

void MapController::selectEvents(const QString& id) {
    std::string std_id = id.toStdString();

    auto current_events_copy = getCurrentEvents();
    clearEvents();

    for (const auto& event : current_events_copy) {
        if (event.getCategories()[0].id == std_id) {
            addEvent(event);
        }
    }
}


Api MapController::getApi() { return api; }
FileHandler MapController::getFh() { return fh; }
std::vector<Category> MapController::getCategories() { return cats; }
std::vector<Event> MapController::getCurrentEvents() { return current_events; }

void MapController::setCategories(std::vector<Category> active) { cats = std::move(active); }
void MapController::setFh(FileHandler f) { fh = std::move(f); }
void MapController::setApi(Api a) { api = std::move(a); }
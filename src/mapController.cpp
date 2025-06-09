#include "mapController.hpp"
#include "nasa_proj/src/event.hpp"
#include "nasa_proj/src/category.hpp"
#include "nasa_proj/src/fileHandler.hpp"

#include <QDebug>
#include <QMap>
#include <QVariant>
#include <QObject>
#include <QVariantList>
#include <QGeoCoordinate>

MapController::MapController(QObject* parent)
    : QObject(parent)
{}

void MapController::addEvent(const Event& event) {

    QString name = QString::fromStdString(event.getTitle());

    const auto& geometryList = event.getGeometry();
    QGeoCoordinate coord;
    QString date = "N/A";
    if (!geometryList.empty()) {
        if (geometryList[0].coordinates.size() < 2) {
            throw std::runtime_error("Invalid coordinates in geometry data for event: " + event.getTitle());
        }
        auto pos = geometryList[0].coordinates;
        coord = QGeoCoordinate(pos[1], pos[0]);
        date = QString::fromStdString(geometryList[0].date);
    }

    QString type = QString::fromStdString(event.getCategories()[0].id);
    QString description = QString::fromStdString(event.getDescription().value_or(""));
    QString link = QString::fromStdString(event.getLink());

    // --- Additional metadata ---
    QString magnitude;
    if (!geometryList.empty()) {
        const auto& geo = geometryList[0];
        if (geo.magnitudeValue.has_value()) {
            magnitude = "Magnitude: " + QString::number(geo.magnitudeValue.value());
            if (geo.magnitudeUnit.has_value()) {
                magnitude += " " + QString::fromStdString(geo.magnitudeUnit.value());
            }
        }
    }

    QStringList sources;
    for (const auto& src : event.getSources()) {
        sources << QString::fromStdString(src.id);
        if (src.url.has_value()) {
            sources.last() += " (" + QString::fromStdString(src.url.value()) + ")";
        }
    }

    QStringList categoryTitles;
    for (const auto& cat : event.getCategories()) {
        categoryTitles << QString::fromStdString(cat.title);
    }

    QString coordsInfo = QString("Location: %1, %2").arg(coord.latitude()).arg(coord.longitude());

    // Don't append extra metadata to description; pass as separate arguments

    static const QMap<QString, QString> typeColorMap = {
        { "wildfires", "#ff4500" },
        { "volcanoes", "#8b0000" },
        { "seaLakeIce", "#00ced1" },
        { "severeStorms", "#9370db" },
        { "drought", "#deb887" },
        { "dustHaze", "#d2b48c" },
        { "earthquakes", "#a9a9a9" },
        { "floods", "#1e90ff" },
        { "landslides", "#a0522d" },
        { "manmade", "#ff1493" },
        { "snow", "#f0f8ff" },
        { "tempExtremes", "#ffa500" },
        { "waterColor", "#7fffd4" }
    };
    QString color = typeColorMap.value(type, "#ff0000"); // fallback to red if not found

    current_events.push_back(event);

    emit addMarker(name, coord, type, color, date, description, link,
                   magnitude, sources.join(", "), categoryTitles.join(", "), coordsInfo);

}

// Downloads the list of categories and stores those that contain events.
// For each category with events, the JSON file is saved and the category is marked active.
// Finally, the list of active categories is applied to the controller.
void MapController::generateCategories(std::string status) {

        api.requestCategories();
        auto categories = api.getData();
        fh.setJData(categories);
        fh.writeToJson("categories.json");


    fh.readFromJson("categories.json");
    auto all_cats = fh.createCategories();
    std::vector<Category> active_cats;

    for (const auto& cat : all_cats) {
        auto filename = cat.getId() + ".json";
        api.requestCategoryById(cat.getId(), status);
        auto data = api.getData();

        if (!data["events"].empty()) {
            fh.setJData(data);
            fh.writeToJson(filename);
            active_cats.push_back(cat);
        } else {
            throw std::runtime_error("No events found for category: " + cat.getId());
        }
    }
    setCategories(active_cats);
}


void MapController::generateEvents(const QString& button_id) {
    QString status_qstr = (button_id.size() >= 17) ? button_id.mid(0, button_id.size() - 17) : "";
    std::string status = status_qstr.toStdString();

    generateCategories(status);
    showEvents(cats);

}


void MapController::showEvents(std::vector<Category> wanted_cats) {
    for (Category cat : wanted_cats) {
        fh.readFromJson(cat.getId() + ".json");
        for (Event event : fh.createEvents()) {
            addEvent(event);
        }
    }
}

void MapController::clearEvents() {
    current_events.clear();       // clear the current event list
    emit clearMarkers();          // notify QML to remove markers from the map
}

void MapController::selectEvents(const QString& id) {
    std::string std_id = id.toStdString();
    std::vector<Category> wanted_cats;

    clearEvents();
    for (Category cat : cats) {
        if (cat.getId() == std_id){
            wanted_cats.push_back(cat);
        }
    }
    showEvents(wanted_cats);
}


void MapController::generateRecentEvents(int days) {
    api.requestEventsDays(days);
    auto data = api.getData();
    fh.setJData(data);
    auto events = fh.createRecentEvents();

    QVariantList simplifiedEvents;
    for (const auto& event : events) {
        if (event.getTitle().empty()) continue;
        if (event.getGeometry().empty()) continue;
        if (event.getGeometry()[0].coordinates.size() < 2) continue;

        recent_events.push_back(event);

        QVariantMap eventMap;
        eventMap["title"] = QString::fromStdString(event.getTitle());
        double lat = std::round(event.getGeometry()[0].coordinates[1] * 1000.0) / 1000.0;
        double lon = std::round(event.getGeometry()[0].coordinates[0] * 1000.0) / 1000.0;
        eventMap["latitude"] = lat;
        eventMap["longitude"] = lon;

        simplifiedEvents.append(eventMap);
    }

    emit recentEventsReady(simplifiedEvents);
}

void MapController::centerMapOn(const QString& title) {
    for (const auto& event : recent_events) {
        if (event.getTitle() == title) {
            addEvent(event);
            return;
        }
    }
    throw std::runtime_error("Event with given title not found: " + title.toStdString());
}

Api& MapController::getApi() { return api; }
FileHandler& MapController::getFh() { return fh; }
std::vector<Category> MapController::getCategories() { return cats; }
std::vector<Event> MapController::getCurrentEvents() { return current_events; }

void MapController::setCategories(std::vector<Category> active) { cats = std::move(active); }
void MapController::setFh(FileHandler f) { fh = std::move(f); }
void MapController::setApi(Api a) { api = std::move(a); }
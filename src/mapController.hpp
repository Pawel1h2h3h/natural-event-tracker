#pragma once

#include "nasa_proj/src/category.hpp"
#include "nasa_proj/src/event.hpp"
#include "nasa_proj/src/fileHandler.hpp"
#include "nasa_proj/src/apikey.hpp"
#include "nasa_proj/src/category.hpp"

#include <iostream>
#include <QObject>
#include <QGeoCoordinate>
#include <QVariant>
#include <QVariantList>
#include <vector>

#/**
# * @class MapController
# * @brief Mediates between the Qt UI and event/category data logic.
# *
# * The MapController class serves as a QObject-based bridge between the frontend interface (e.g., QML/Qt GUI)
# * and the backend logic for retrieving, managing, and visualizing event and category data on a map.
# * It manages interaction logic such as filtering events, generating categories, centering on specific locations,
# * and updating map markers. It also emits signals to update the UI with event markers or clear the view.
# *
# * The controller integrates with the Api and FileHandler components to provide a complete data pipeline.
# */
class MapController : public QObject {
    Q_OBJECT
public:
    explicit MapController(QObject *parent = nullptr);

    Q_INVOKABLE void addEvent(const Event& event);
    Q_INVOKABLE void generateCategories(std::string status);
    Q_INVOKABLE void generateEvents(const QString& button_id);
    Q_INVOKABLE void clearEvents();
    Q_INVOKABLE void selectEvents(const QString& id);
    Q_INVOKABLE void centerMapOn(const QString& title);

    Q_INVOKABLE void generateRecentEvents(int days);

    Api& getApi();
    FileHandler& getFh();
    std::vector<Category> getCategories();
    std::vector<Event> getCurrentEvents();

    void showEvents(std::vector<Category> wanted_cats);
    // void generateEventsDays(int days);
    void setCategories(std::vector<Category> active);
    void setFh(FileHandler f);
    void setApi(Api a);

signals:
    void addMarker(const QString& name,
        const QGeoCoordinate& coord,
        const QString& type,
        const QString& color,
        const QString& date,
        const QString& description,
        const QString& link,
        const QString& magnitude,
        const QString& sources,
        const QString& categories,
        const QString& coordinates);
    void clearMarkers();

    void recentEventsReady(QVariantList recentEvents);


private:
    std::vector<Event> current_events;
    std::vector<Event> recent_events;
    std::vector<Category> cats;
    FileHandler fh;
    Api api;
};
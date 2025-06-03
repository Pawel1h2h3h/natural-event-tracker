#pragma once

#include "nasa_proj/src/category.hpp"
#include "nasa_proj/src/event.hpp"
#include "nasa_proj/src/fileHandler.hpp"
#include "nasa_proj/src/apikey.hpp"
#include "nasa_proj/src/category.hpp"

#include <iostream>
#include <QObject>
#include <QGeoCoordinate>
#include <vector>

class MapController : public QObject {
    Q_OBJECT
public:
    explicit MapController(QObject *parent = nullptr);

    Q_INVOKABLE void addEvent(const Event& event);
    Q_INVOKABLE void generateExistingCategories();
    Q_INVOKABLE void generateAllEvents();
    Q_INVOKABLE void clearEvents();
    Q_INVOKABLE void selectEvents(const QString& id);

    Api getApi();
    FileHandler getFh();
    std::vector<Category> getCategories();
    std::vector<Event> getCurrentEvents();

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
        const QString& link);
    void clearMarkers();


private:
    std::vector<Event> current_events;
    std::vector<Category> cats;
    FileHandler fh;
    Api api;
};
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "mapController.hpp"

using namespace Qt::StringLiterals;


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MapController mapController;
    engine.rootContext()->setContextProperty("mapController", &mapController);

    const QUrl url(u"qrc:/qt/qml/Main/main.qml"_s);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);
    mapController.generateRecentEvents(7);

    return app.exec();
}
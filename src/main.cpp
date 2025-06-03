#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mapController.hpp"



int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    MapController mapController;
    engine.rootContext()->setContextProperty("mapController", &mapController);

    const QUrl url(u"qrc:/qt/qml/Main/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.load(url);

    mapController.generateExistingCategories();


    return app.exec();
}
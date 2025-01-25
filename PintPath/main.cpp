#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "pintbackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<PintBackend>("PintPath", 1, 0, "PintBackend");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("PintPath", "Main");

    return app.exec();
}

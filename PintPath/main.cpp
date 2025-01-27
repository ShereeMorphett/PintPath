#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "pintbackend.h"
#include "vendormodel.h"

//TODO: fix style conventions in the CPP vendor_model    pint_backend
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<PintBackend>("PintPath", 1, 0, "PintBackend");

    // Register VendorModel as an uncreatable type
    qmlRegisterUncreatableType<VendorModel>(
        "VendorModel",
        1,
        0,
        "VendorModel",
        "VendorModel cannot be created in QML. Use PintBackend to access it.");

    PintBackend backend;
    engine.rootContext()->setContextProperty("vendorModel", backend.getVendorModel());

    // Connect engine errors to application exit
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // Load the QML module
    engine.loadFromModule("PintPath", "Main");

    return app.exec();
}

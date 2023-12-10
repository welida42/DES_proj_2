// Necessary Qt includes for our application
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "main_fun.h"

// CanReceiver class constructor definition
CanReceiver::CanReceiver() {
    // Register the D-Bus service and the D-Bus object for our application
    QDBusConnection::sessionBus().registerService("com.example.CanData");
    QDBusConnection::sessionBus().registerObject("/com/example/CanData/Data", this, QDBusConnection::ExportAllSlots);
}

// Getter function for speed
double CanReceiver::speed() const {
    return m_speed;
}

// Getter function for rpm
double CanReceiver::rpm() const {
    return m_rpm;
}

// Getter function for battery
double CanReceiver::battery1() const {
    return m_battery1;
}

double CanReceiver::battery2() const {
    return m_battery2;
}

double CanReceiver::battery3() const {
    return m_battery3;
}

// Method to set the speed, rpm and battery data, and emit signals when they change
void CanReceiver::setData(double speed, double rpm, double battery1, double battery2, double battery3) {
    m_speed = speed;
    m_rpm = rpm;
    m_battery1 = int(battery1);
    m_battery2 = int(battery2);
    m_battery3 = int(battery3);

    qDebug() << speed;
    qDebug() << rpm;
    qDebug() << battery1;
    qDebug() << battery2;
    qDebug() << battery3;

    emit speedChanged(); // Notify any listeners that speed has changed
    emit rpmChanged();   // Notify any listeners that rpm has changed
    emit battery1Changed();   // Notify any listeners that battery has changed
    emit battery2Changed();
    emit battery3Changed();
}


// Main function for the application
int main(int argc, char *argv[])
{
    // Enable High DPI scaling for devices that support it
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Create a QGuiApplication object for our GUI application
    QGuiApplication app(argc, argv);

    // Create a QQmlApplicationEngine object for our QML engine
    QQmlApplicationEngine engine;

    // Instantiate our CanReceiver class
    CanReceiver canReceiver;
    // Expose our CanReceiver instance to QML as "canReceiver"
    engine.rootContext()->setContextProperty("canReceiver", &canReceiver);

    // Load our main QML file
    const QUrl url(QStringLiteral("qrc:/main_fun.qml"));
    // Connect a signal to handle if object creation fails in the QML engine
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        // If object creation fails, exit the application
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // Load the QML file into our engine
    engine.load(url);

    // Run our application's event loop
    return app.exec();
}

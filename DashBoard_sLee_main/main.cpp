#include <cardash.h>

// This is the main function of the program.
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);            // This line enables high DPI scaling for Qt versions less than 6.0.0.
#endif

    QGuiApplication         app(argc, argv);                                // This line creates a new QGuiApplication object.;

    QQmlApplicationEngine   engine;                                         // This line creates a new QQmlApplicationEngine object.

    carDash                 CarDash;                                        // This line creates a new carDash object.

    engine.rootContext()    -> setContextProperty("CarDash", &CarDash);     // This line sets the carDash object as a context property for the QQmlApplicationEngine.

    const QUrl              url(QStringLiteral("qrc:/main.qml"));           // This line sets the URL of the main QML file.

    // This line connects the objectCreated signal of the QQmlApplicationEngine to a lambda function that exits the application if the root object is not created.
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);                                                       // This line loads the main QML file into the QQmlApplicationEngine.

    return                  app.exec();                                     // This line starts the event loop of the QGuiApplication.
}

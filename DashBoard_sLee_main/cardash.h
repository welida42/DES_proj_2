#ifndef CARDASH_H
#define CARDASH_H

#include <QObject>
#include <QDBusConnection>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QMutex>
#include <dash_adaptor.h>

class carDash   :   public QObject
{
    Q_OBJECT

    Q_PROPERTY                              (double speed READ speed NOTIFY speedChanged)
    Q_PROPERTY                              (double rpm READ rpm NOTIFY rpmChanged)
    Q_PROPERTY                              (double battery READ battery NOTIFY batteryChanged)
    Q_PROPERTY                              (QString gear READ gear NOTIFY gearChanged)
    Q_PROPERTY                              (int errval READ errval NOTIFY errchk)

public:
    carDash();                                                                                              // This is the constructor. It sets the initial values of the car's speed, rpm, battery, gear, and error value.

    double      speed()                     const;                                                          // This function returns the current speed of the car.
    double      rpm()                       const;                                                          // This function returns the current rpm of the car.
    double      battery()                   const;                                                          // This function returns the current battery of the car.
    QString     gear()                      const;                                                          // This function returns the current gear of the car.
    int         errval()                    const;                                                          // This function returns the current error value.

signals:
    void        speedChanged                (double speed);                                                 // This signal is emitted when the speed of the car changes.
    void        rpmChanged                  (double rpm);                                                   // This signal is emitted when the rpm of the car changes.
    void        batteryChanged              (double battery);                                               // This signal is emitted when the battery percentage of the car changes.
    void        gearChanged                 (QString gear);                                                 // This signal is emitted when the gear

    void        errchk                      ();                                                             // This signal is emitted when the error value should be checked.

public slots:
    void        setData                     (double speed, double rpm, double battery, QString gear);       // This function sets the new values of the car's speed, rpm, battery, and gear, and emits signals that these values have changed.

    void        updateSpeed                 (double speed);                                                 // This function updates the speed of the car.
    void        updateRpm                   (double rpm);                                                   // This function updates the rpm of the car.
    void        updateBattery               (double battery);                                               // This function updates the battery percentage of the car.
    void        updateGear                  (QString gear);                                                 // This function updates the gear of the car.

    void        checkErrVal                 ();                                                             // This function continuously checks the error value and calls the handleErrorOnPython function if necessary.
    void        startThread                 ();                                                             // This function starts a new thread to check the error value.
    void        callHandleErrorOnPython     ();                                                             // This function calls the handleError function in a Python program via the D-Bus.

private:
    double      my_speed;                                                                                   // This is the current speed of the car.
    double      my_rpm;                                                                                     // This is the current rpm of the car.
    double      my_battery;                                                                                 // This is the current battery percentage of the car.
    QString     my_gear;                                                                                    // This is the current gear of the car.

    int         my_errval;                                                                                  // This is the current error value.
    int         previous_errval             = 0;                                                            // This is the previous error value.
    int         my_errvalSwitchCount        = 0;                                                            // This is the count of how many times the error value has switched between 1 and 2.
    QThread*    errvalCheckThread;                                                                          // This is the thread that checks the error value.
    QMutex      mutex;                                                                                      // This is a mutex used to lock the error value when it is being updated.

};

#endif // CARDASH_H

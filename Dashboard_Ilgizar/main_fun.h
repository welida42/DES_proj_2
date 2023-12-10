// Prevents the header from being included multiple times in a single compilation.
#ifndef MAIN_FUN_H
#define MAIN_FUN_H

// Necessary includes for the class.
#include <QObject>           // Base class for objects in Qt's object system.
#include <QDBusConnection>   // Provides a connection to the D-Bus messaging system.

// Declare a class named CanReceiver that inherits from QObject.
class CanReceiver : public QObject
{
    // Qt's macro to enable meta-object features like signals, slots, and properties.
    Q_OBJECT

    // Property declarations for the class.
    // They can be accessed from QML or any Qt meta-object system.
    // It has a READ accessor and a signal to notify when its value changes.
    Q_PROPERTY(float speed READ speed NOTIFY speedChanged)   // Property for speed.
    Q_PROPERTY(float rpm READ rpm NOTIFY rpmChanged)         // Property for rpm.
    Q_PROPERTY(float battery1 READ battery1 NOTIFY battery1Changed) // Property for battery.
    Q_PROPERTY(float battery2 READ battery2 NOTIFY battery2Changed)
    Q_PROPERTY(float battery3 READ battery3 NOTIFY battery3Changed)

public:
    // Constructor declaration.
    CanReceiver();

    // Accessor methods (getters) for the properties.
    double speed() const;    // Getter for the 'speed' property.
    double rpm() const;      // Getter for the 'rpm' property.
    double battery1() const;      // Getter for the 'battery' property.
    double battery2() const;
    double battery3() const;

public slots:
    // Slot declaration. Slots are methods that can be invoked in response to signals.
    // In the context of D-Bus, this can also be a method that can be invoked over the bus.
    void setData(double speed, double rpm, double battery1, double battery2, double battery3);

signals:
    // Signal declarations. Signals are emitted to notify other parts of the application of certain events.
    void speedChanged();    // Signal emitted when the 'speed' property changes.
    void rpmChanged();      // Signal emitted when the 'rpm' property changes.
    void battery1Changed();      // Signal emitted when the 'battery' property changes.
    void battery2Changed();
    void battery3Changed();

private:
    // Private member variables to store the actual data.
    double m_speed;         // Holds the speed data.
    double m_rpm;           // Holds the rpm data.
    double m_battery1;           // Holds the battery data.
    double m_battery2;
    double m_battery3;
};

// Ends the header guard.
#endif // MAIN_FUN_H

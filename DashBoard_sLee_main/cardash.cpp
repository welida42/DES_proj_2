#include "cardash.h"

// This is the constructor of the carDash class. It sets the initial values of the car's speed, rpm, battery, gear, and error value.
carDash::carDash():     my_speed(0), my_rpm(0),my_battery(0), my_gear("OFF"), my_errval(-1)
{

    // Connect this program to the D-Bus and make it known as "com.example.CanData".
    QDBusConnection::sessionBus().registerService("com.example.CanData");

    // Create a new CarDashAdaptor object and associate it with this object.
    new CarDashAdaptor(this);

    // Register this object on the D-Bus so that other programs can use it.
    QDBusConnection::sessionBus().registerObject("/com/example/CanData/Data", this);

    // Start a new thread to check the error value.
    startThread();

    // Connect the signals to the slots. This means that when the speed, rpm, battery, or gear changes, the corresponding update function will be called.
    connect             (this, SIGNAL(speedChanged(double)), this, SLOT(updateSpeed(double)));
    connect             (this, SIGNAL(rpmChanged(double)), this, SLOT(updateRpm(double)));
    connect             (this, SIGNAL(batteryChanged(double)), this, SLOT(updateBattery(double)));
    connect             (this, SIGNAL(gearChanged(QString)), this, SLOT(updateGear(QString)));

}

// This function returns the current speed of the car.
double                  carDash::speed()        const
{
    return              my_speed;
}


// This function returns the current rpm of the car.
double                  carDash::rpm()          const
{
    return              my_rpm;
}


// This function returns the current battery percentage of the car.
double                  carDash::battery()      const
{
    return              my_battery;
}


// This function returns the current gear of the car.
QString                 carDash::gear()         const
{
    return              my_gear;
}

// This function returns the current error value.
int                     carDash::errval()       const
{
    return              my_errval;
}

// This function updates the speed of the car.
void                    carDash::updateSpeed(double speed)
{
    my_speed            = int(speed);
}

// This function updates the rpm of the car.
void                    carDash::updateRpm(double rpm)
{
    my_rpm              = std::round(rpm*10)/ 10.0;
}


// This function updates the battery percentage of the car.
void                    carDash::updateBattery(double battery)
{
    my_battery          = int(battery);

}

// This function updates the gear of the car.
void                    carDash::updateGear(QString gear)
{
    my_gear             = gear;
}

// This function sets the new values of the car's speed, rpm, battery, and gear, and emits signals that these values have changed.
void                    carDash::setData(double my_speed, double my_rpm, double my_battery, QString my_gear)
{

    if(!std::isnan(my_speed))
    {
                        emit speedChanged(my_speed);
    }
    if(!std::isnan(my_rpm))
    {
                        emit rpmChanged(my_rpm);
    }
    if(!std::isnan(my_battery))
    {
                        emit batteryChanged(my_battery);
    }
    if(1)
    {
                        emit gearChanged(my_gear);
    }

    // Change the error value and reset the error value switch count.
    {
                        QMutexLocker locker(&mutex);
                        my_errval           = (my_errval == 1) ? 2 : 1;
                        my_errvalSwitchCount                        = 0;
    }

    // Emit a signal that the error value should be checked.
    emit                errchk();
}

// This function starts a new thread to check the error value.
void                    carDash::startThread()
{
    errvalCheckThread   = QThread::create([this] { checkErrVal(); });
    errvalCheckThread   -> start();
}

// This function continuously checks the error value and calls the handleErrorOnPython function if necessary.
void                    carDash::checkErrVal()
{
    while(true)
    {

                        QThread::sleep(1);

                        qDebug() << "Error Count: " << my_errvalSwitchCount;

                        if (my_errval == previous_errval)
                        {
                                            my_errvalSwitchCount++;
                        }

                        if  (3 <my_errvalSwitchCount && my_errvalSwitchCount < 7)
                        {
                                            callHandleErrorOnPython();
                        }

                        if  (my_errvalSwitchCount >= 7)
                        {
                                            my_errval                                   = -1;
                                            emit                                        errchk();
                                            emit                                        speedChanged(-1);
                                            emit                                        rpmChanged(my_rpm);
                                            emit                                        batteryChanged(-1);
                                            emit                                        gearChanged("OFF");
                                            QThread::sleep(2);
                                            QCoreApplication::exit(0);
                        }

                        {
                                            QMutexLocker locker(&mutex);
                                            previous_errval                             = my_errval;
                        }
    }
}

// This function calls the handleError function in a Python program via the D-Bus.
void                    carDash::callHandleErrorOnPython()
{
    QDBusInterface      callPy("com.example.CanData", "/com/example/CanData/Chkout", "com.example.Chkout");

    if (callPy.isValid())
    {
                        callPy.call("handleError");
    }
    else
    {
                        qDebug() << "Failed to access the D-Bus interface!";
    }
}

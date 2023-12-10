TEMPLATE = app

QT += qml quick core gui dbus

CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS

# Input
SOURCES += \
        main.cpp \
        speedometer.cpp
 \        #main_fun.cpp


HEADERS += \
        main.h \
        speedometer.h
 \        #main_fun.h

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Des igner
QML_DESIGNER_IMPORT_PATH =

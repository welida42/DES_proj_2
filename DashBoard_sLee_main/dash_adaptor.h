/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -a dash_adaptor dash.xml
 *
 * qdbusxml2cpp is Copyright (C) 2023 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DASH_ADAPTOR_H
#define DASH_ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface local.carDash
 */
class CarDashAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "local.carDash")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"local.carDash\">\n"
"    <method name=\"setData\">\n"
"      <arg direction=\"in\" type=\"d\" name=\"speed\"/>\n"
"      <arg direction=\"in\" type=\"d\" name=\"rpm\"/>\n"
"      <arg direction=\"in\" type=\"d\" name=\"battery\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"gear\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    CarDashAdaptor(QObject *parent);
    virtual ~CarDashAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void setData(double speed, double rpm, double battery, const QString &gear);
Q_SIGNALS: // SIGNALS
};

#endif

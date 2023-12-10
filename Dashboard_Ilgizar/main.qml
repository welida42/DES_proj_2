import QtQuick 2.9
import QtQuick.Controls 2.15
import com.welida.speedometer 1.0
import QtQuick.Layouts 1.1

ApplicationWindow {
    visible: true
    width: 1280
    height: 400
//    title: "CAN Data Viewer"
    color: "black"


    Speedometer
    {
        objectName: "speedoMeter"
        anchors.horizontalCenter:  parent.horizontalCenter
        width: speedometerSize
        height: speedometerSize
        startAngle: startAngle
        alignAngle: alignAngle
        lowestRange: lowestRange
        highestRange: highestRange
        speed: speed
        battery: battery
        arcWidth: arcWidth
        outerColor: outerColor
        innerColor: innerColor
        textColor: textColor
        backgroundColor: backgroundColor
    }
}




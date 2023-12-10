import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15

Window {

    width: 1280
    height: 400
    visible: true
    title: qsTr("PiRacer_DashBoard")
    visibility: Window.FullScreen

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        color: "black"
        opacity: 1
        visible: true

        //Gear Gauge Section

        Image{
            id: gearGauge
            x: 32
            y: 207
            width: 300
            height: 400
            source: "DashBoardImg/gauge-gauge-frame.png"
            rotation: 91.599


            Text {
                id: pGear
                x: 44
                y: 150
                text: "P"
                color: CarDash.gear === "P" ? "blue" : "white"
                font.pixelSize: 14
                rotation: -91.183
            }

            Text {
                id: rGear
                x: 57
                y: 108
                text: "R"
                color: CarDash.gear === "R" ? "blue" : "white"
                font.pixelSize: 14
                rotation: -92.41
            }

            Text {
                id: nGear
                x: 77
                y: 67
                text: "N"
                color: CarDash.gear === "N" ? "blue" : "white"
                font.pixelSize: 14
                rotation: -91.43
            }

            Text {
                id: dGear
                x: 108
                y: 33
                text: "D"
                color: CarDash.gear === "D" ? "blue" : "white"
                font.pixelSize: 14
                rotation: -90.857
            }

            Text {
                id: mainGear
                x: 87
                y: 115
                text : CarDash.gear
                font.pixelSize: 25
                rotation: 268.792
                color: "white"
            }

        }

        Image {
            id: mainCarImg
            source: "DashBoardImg/bg-mask.png"
            anchors.verticalCenterOffset: 100
            anchors.horizontalCenterOffset: -555
            anchors.centerIn: parent
            width: 300
            height: 200

            Image {
                id: mainCarHighlights
                x: -42
                y: -18
                width: 383
                height: 226
                source: "DashBoardImg/car-highlights.png"
            }

            Image {
                id: centerGauge
                x: 22
                y: 54
                width: 260
                height: 260
                source: "DashBoardImg/gauge-gauge-frame-sport-center.png"
                rotation: 48.159
            }

            Image {
                id: mainCarHighlightsSurrond
                source: "DashBoardImg/car-highlights.png"
                x: -255
                y: -218
                width: 809
                height: 538
            }

        }


        Image {
            id: speedGauge
            x: -48
            y: -60
            width: 380
            height: 340
            horizontalAlignment: Image.AlignHCenter
            verticalAlignment: Image.AlignTop
            source: "DashBoardImg/gauge-gauge-frame-sport-side.png"
            rotation: 232.856


            Text{
                id: zero
                x: 182
                y: 14
                text: "0"
                color: "white"
                font.pixelSize: 14
                rotation: -52.905
            }
            Text{
                id: twenty
                x: 116
                y: 28
                text: "20"
                color: "white"
                font.pixelSize: 14
                rotation: 127.873
            }
            Text{
                id: fourty
                x: 68
                y: 69
                text: "40"
                color: "white"
                font.pixelSize: 14
                rotation: -233.911
            }
            Text{
                id: sixty
                x: 48
                y: 127
                text: "60"
                color: "white"
                font.pixelSize: 14
                rotation: -232.602
            }
            Text{
                id: eighty
                x: 48
                y: 186
                text: "80"
                color: "white"
                font.pixelSize: 14
                rotation: -232.508
            }
            Text{
                id:hundred
                x: 62
                y: 241
                text: "100"
                color: "white"
                font.pixelSize: 14
                rotation: 127.072
            }
            Text{
                id: onetwenty
                x: 110
                y: 283
                text: "120"
                color: "white"
                font.pixelSize: 14
                rotation: 125.805
            }
            Text{
                id: onefourty
                x: 169
                y: 304
                text: "140"
                color: "white"
                font.pixelSize: 14
                rotation: -233
            }

            Text{
                id: currentSpeed
                x: 182
                y: 111
                text: CarDash.speed
                color: "white"
                font.pixelSize: 20
                anchors.horizontalCenterOffset: 56
                rotation: 127.869
                anchors.horizontalCenter: parent.horizontalCenter
            }


            property real needleAngle: (CarDash.speed / 140) * -180
            property int update_interval: 200

            Image{
                id: speedGaugeNeedle
                x: 157
                y: -1
                width: 57
                height: 214
                source: "DashBoardImg/needle-standard-sport.png"
                transformOrigin: Item.Left
                transform: Rotation{
                    origin.x: speedGaugeNeedle.width / 2
                    origin.y: speedGaugeNeedle.height * 3 / 4
                    angle: speedGauge.needleAngle
                    Behavior on angle { RotationAnimation { duration: speedGaugeNeedle.update_interval} }

                }

            }

            Item {
                x: -249
                y: 221
                width: 110
                height: 32
                rotation: -233

                Rectangle {
                    color: "#002b66"
                    height: 6
                    width: 100
                    y: 19
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    visible: CarDash.battery > 0
                    anchors.right: parent.right
                    anchors.rightMargin: 11

                    Rectangle {
                        color: "green";
                        height: parent.height;
                        width: Math.max(0.7 * CarDash.battery, 0);
                    }
                }

                Text {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    text: Math.floor(CarDash.battery) + "%"
                    color: "white"
                    font.pixelSize: 12
                    font.family: "Sarabun"
                }

                Image {
                    id: batteryImage
                    y: 10
                    width: 17
                    height: 15
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    source: "DashBoardImg/battery.png"
                    anchors.bottomMargin: 7
                    rotation: 0

                    anchors.leftMargin: 9
                }


            }

            Image {
                id: connectStatus
                x: 113
                y: 355
                width: 25
                height: 25
                source: "DashBoardImg/Warning.png"
                rotation: -232.884
                visible: CarDash.errval === -1 // Use speed as an indicator of D-Bus connection status

                Text{
                    id: connectStatus_t
                    x: 3
                    y: -19
                    text: "D-Bus Connection Error"
                    color : "red"
                    visible: CarDash.errval === -1
                }
            }
        }
    }
}



/*##^##
Designer {
    D{i:0;formeditorZoom:1.75}
}
##^##*/

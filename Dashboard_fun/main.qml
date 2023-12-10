import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 1280
    height: 400
    title: "CAN Data Viewer"

    Item {
        id: rootItem
        property int currentImageIndex: 0
        property var imageSources: [
            "images/hubo3-0.png",
            "images/hubo3-1.png",
            "images/hubo3-2.png",
            "images/hubo3-3.png",
            "images/hubo3-4.png",
            "images/hubo3-5.png",
            "images/hubo3-6.png",
            "images/hubo3-7.png",
            "images/hubo3-8.png",
            "images/hubo3-9.png",
            "images/hubo3-10.png",
            "images/hubo3-11.png"
        ]

        Image {
            id: animatedImage
            x: 450
            y: 50
            width: 300
            height: 300
            source: parent.imageSources[parent.currentImageIndex]

            Timer {
                id: imageTimer
                interval: 1000 - (canReceiver.speed * 9)  //Mininum: 1ms, Maximum: 1000ms

                running: true
                repeat: true

                onTriggered: {
                    rootItem.currentImageIndex = (rootItem.currentImageIndex + 1) % rootItem.imageSources.length;
                    animatedImage.source = rootItem.imageSources[rootItem.currentImageIndex];
                    console.log("Image source changed to: " + canReceiver.speed);
                }
            }
        }

        Column {
            x: 20
            y: 20

            Text {
                text: "Speed: " + canReceiver.speed + " km/h"
                font.pixelSize: 24
            }

            Text {
                text: "RPM: " + canReceiver.rpm
                font.pixelSize: 24
            }

            Text {
                text: "Battery: " + canReceiver.battery + "%"
                font.pixelSize: 24
            }
        }
    }
}




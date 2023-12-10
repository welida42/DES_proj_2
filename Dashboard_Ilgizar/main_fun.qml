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
//        property var imageSources: [
//            "images/hubo3-0.png",
//            "images/hubo3-1.png",
//            "images/hubo3-2.png",
//            "images/hubo3-3.png",
//            "images/hubo3-4.png",
//            "images/hubo3-5.png",
//            "images/hubo3-6.png",
//            "images/hubo3-7.png",
//            "images/hubo3-8.png",
//            "images/hubo3-9.png",
//            "images/hubo3-10.png",
//            "images/hubo3-11.png"
//        ]

        property var imageSources: [
            "images/hubo4-1.png",
            "images/hubo4-2.png",
            "images/hubo4-3.png",
            "images/hubo4-4.png",
            "images/hubo4-5.png",
            "images/hubo4-6.png",
            "images/hubo4-7.png",
            "images/hubo4-8.png",
            "images/hubo4-9.png",
            "images/hubo4-10.png",
            "images/hubo4-11.png",
            "images/hubo4-12.png",
            "images/hubo4-13.png",
            "images/hubo4-14.png",
            "images/hubo4-15.png",
            "images/hubo4-16.png",
            "images/hubo4-17.png",
            "images/hubo4-18.png",
            "images/hubo4-19.png",
            "images/hubo4-20.png",
            "images/hubo4-21.png",
            "images/hubo4-22.png",
            "images/hubo4-23.png",
            "images/hubo4-24.png"
        ]

        property var gifSources: [
            "images/bat_0.gif",
            "images/bat_1.gif",
            "images/bat_2.gif",
            "images/bat_3.gif",
            "images/bat_4.gif",
            "images/bat_5.gif",
            "images/bat_6.gif",
            "images/bat_7.gif",
            "images/bat_8.gif",
            "images/bat_9.gif",
        ]


        Image {
            source: "images/background.png"
            width: 1280
            height: 400
        }

        Image {
            id: animatedImage
            x: 350
            y: -70
            width: 500
            height: 500
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

        AnimatedImage {
            id: battertyGif3
            width: 150
            height: 150
            x: 1100
            y: 120

            source: parent.gifSources[canReceiver.battery3]
        }
        AnimatedImage {
            id: battertyGif2
            width: 150
            height: 150
            x: 980
            y: 120

            source: parent.gifSources[canReceiver.battery2]
        }
        AnimatedImage {
            id: battertyGif1
            width: 150
            height: 150
            x: 860
            y: 120

            source: parent.gifSources[canReceiver.battery1]
        }
    }
}



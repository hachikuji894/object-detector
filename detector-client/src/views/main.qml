import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.2
import QtMultimedia 5.12
import QtQuick.Controls.Material 2.12
import QtGraphicalEffects 1.12


import "qrc:/views/screen" as Screens
import "qrc:/views/widget" as Widgets

import "qrc:/views/screen" as MediaScreen


ApplicationWindow
{
    id: application

    property color detectionRectanglesColor: "white"

    visible: true
    property double screenFactor: 0.8

    // fix the widget
    minimumWidth: Screen.width * screenFactor
    maximumWidth: Screen.width * screenFactor
    minimumHeight: Screen.height * screenFactor
    maximumHeight: Screen.height * screenFactor
    title: qsTr("Object Detector Client")


    Timer {
        id: loadImageTimer
        property var url;
        interval: 1000; running: false; repeat: true
        onTriggered: {
            httpManager.processNetworkImage(url)
        }
    }

    MediaScreen.MediaOutputPane{
        id: mainMediaPanel
        visible: false
        anchors{
            top: parent.top
            left: parent.left
        }
        height: parent.height
        width: parent.width
    }

    Component {
        id: mediaScreen
        MediaScreen.MediaScreen{
            anchors.margins: 5
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mediaScreen
    }

    footer:
        Widgets.MenuTapBar{
        id: footer
        visible: true
    }
}
import QtQuick 2.12
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import QtGraphicalEffects 1.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls.Material.impl 2.12
import QtQuick.Controls.Styles 1.4


import "qrc:/views/widget" as Widgets
Item {
    Widgets.ColorHelper{id: colorHelper}
    Pane{
        id: root
        property string tipsImage: "             Please select a file."
        property string tipsVideo: "             Please select a file."


        anchors.fill: parent

        Material.elevation: 6
        Material.background: Material.White

        function setMediaButtonState(playing){
            if(playing===false){
                mediaStateButton.icon.source = "qrc:/assets/image/play.png"
                mediaStateButton.text = qsTr("Play media")
                mainMediaPanel.stop()

            }else{
                mediaStateButton.icon.source = "qrc:/assets/image/stop.png"
                mediaStateButton.text = qsTr("Stop media")
                mainMediaPanel.play()
            }
        }

        ColumnLayout{
            ButtonGroup { id: mediaSelectorRadioButtons }

            ColumnLayout {
                RowLayout{
                    opacity: 0.5
                    Image {
                        sourceSize.width: 40
                        source: "qrc:/assets/image/round_computer_black_48dp.png"
                    }
                    Label{
                        text: "LOCAL MEDIA"
                    }
                }

                Widgets.SeparatorLine{}

                RowLayout{
                    Layout.leftMargin: 5
                    Image {
                        opacity: 0.5
                        sourceSize.width: 30
                        source: "qrc:/assets/image/round_insert_photo_black_48dp.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: videoMediaSelectionRadiobutton_localImage.checked = !videoMediaSelectionRadiobutton_localImage.checked
                        }
                    }
                    RadioButton {
                        id: videoMediaSelectionRadiobutton_localImage
                        ButtonGroup.group: mediaSelectorRadioButtons
                        text: qsTr("Image")
                        checked: mainMediaPanel.mediaSource === "image"
                        onCheckedChanged: {
                            if(checked){
                                //loadImageTimer.start()
                                mediaStateButton.visible = false;
                                lastSeparatorLine.visible = false;
                            }
                            else{
                                //loadImageTimer.stop()
                                mediaStateButton.visible = true;
                                lastSeparatorLine.visible = true;
                            }
                        }

                        onClicked: {
                            root.setMediaButtonState(false)

                            mainMediaPanel.mediaSource = "image"
                            if(root.tipsImage !== imageLoader.fileText){
                                mainMediaPanel.mediaUrl = imageLoader.fileText
                                loadImageTimer.url = imageLoader.fileText
                                mainMediaPanel.play()
                            }
                        }
                    }
                    Widgets.FileLoader{
                        id: imageLoader
                        labelText: ""
                        Layout.fillWidth: true
                        fileText: root.tipsImage //fileHandler.getCurrentPath()
                        nameFilters: ["*.*"]
                        function onLoaded(){
                            fileText=file
                            //onEnterPressedFnc()
                        }
                        function onEnterPressedFnc(){
                            videoMediaSelectionRadiobutton_localImage.clicked()
                            root.setMediaButtonState(true)
                        }
                        onFileChoosen: onLoaded
                        onEnterPressed: onEnterPressedFnc
                    }
                }

                RowLayout{
                    Layout.leftMargin: 5
                    Image {
                        opacity: 0.5
                        sourceSize.width: 30
                        source: "qrc:/assets/image/round_local_movies_black_48dp.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: videoMediaSelectionRadiobutton_localVideo.checked = !videoMediaSelectionRadiobutton_localVideo.checked
                        }
                    }
                    RadioButton {
                        id: videoMediaSelectionRadiobutton_localVideo
                        ButtonGroup.group: mediaSelectorRadioButtons
                        text: qsTr("Video ")
                        checked: mainMediaPanel.mediaSource === "video"
                        onClicked: {
                            root.setMediaButtonState(false)
                            mainMediaPanel.mediaSource = "video"
                            if(root.tipsVideo !== videoLoader.fileText){
                                mainMediaPanel.mediaUrl = videoLoader.fileText
                            }
                        }
                    }
                    Widgets.FileLoader{
                        id: videoLoader
                        labelText: ""
                        fileText: root.tipsVideo //fileHandler.getCurrentPath()
                        Layout.fillWidth: true
                        nameFilters: ["*.*"]
                        function onLoaded(){
                            fileText=file
                            //onEnterPressedFnc()
                        }
                        function onEnterPressedFnc(){
                            videoMediaSelectionRadiobutton_localVideo.clicked()
                            root.setMediaButtonState(true)
                        }
                        onFileChoosen: onLoaded
                        onEnterPressed: onEnterPressedFnc
                    }
                }
                RowLayout{
                    Layout.leftMargin: 5
                    Image {
                        opacity: 0.5
                        sourceSize.width: 30
                        source: "qrc:/assets/image/camera_black.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: videoMediaSelectionRadioButton_Camera.checked = !videoMediaSelectionRadioButton_Camera.checked
                        }
                    }
                    RadioButton {
                        id: videoMediaSelectionRadioButton_Camera
                        ButtonGroup.group: mediaSelectorRadioButtons
                        text: qsTr("Device camera")
                        checked: mainMediaPanel.mediaSource === "camera"
                        onClicked: {
                            root.setMediaButtonState(false)
                            mainMediaPanel.mediaSource = "camera"
                        }
                    }
                    ComboBox{
                        Material.accent: Material.Orange
                        Material.foreground: Material.BlueGrey
                        model: ListModel{
                            ListElement {
                                displayName: "USB2.0 HD UVC WebCam"
                                deviceId: 0
                            }
                        }
                        textRole: "displayName"
                        onActivated:{}
                    }
                }

                Widgets.SeparatorLine{}











                // Player Button in the bottom

                Item{
                    Layout.fillHeight: true
                }

                Widgets.SeparatorLine{
                    id: lastSeparatorLine
                }
                ToolButton{
                    visible: true

                    id: mediaStateButton
                    Layout.alignment: Qt.AlignCenter

                    text: qsTr("Stop media")
                    icon.source: "qrc:/assets/image/stop.png"
                    font.family: "Helvetica"
                    antialiasing: true
                    Layout.fillWidth: true
                    onClicked: {
                        root.setMediaButtonState(text==="Play media"?true:false)
                    }
                }
            }
            anchors.fill: parent
        }
    }
}

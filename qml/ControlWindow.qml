import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    id: win

    property var heads: []
    property int head_count: 0

    MouseArea { anchors.fill: parent }

    function create_robot_head(name) {
        var newHead


        newHead = headDelegate.createObject(headsLayout,
                                            {"id": "head_" + head_count.toString(),
                                                "x": head_count*100 + 5*(head_count+1),
                                                "text": name
                                            });
        heads.push(newHead)
        head_count++
    }

    Component.onCompleted: {
        create_robot_head('Dummy')
    }

    Connections {
        target: btController
        onDeviceConnected: {
            create_robot_head(name)
        }
    }

    Rectangle {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        height: 50
        color: defaultColor
        border.width: 2
        radius: 5
        Item {
            id: headsLayout
            anchors.fill: parent
        }

    }

    Component {
        id: headDelegate
        Rectangle {
            id: headDelegateRect
            color: "lime"
            border.color: "black"
            border.width: 2
            radius: 5
            width: 100
            height: 40
            y: 5
            property alias text: headText.text
            Image {
                id: headImage
                fillMode: Image.PreserveAspectFit
                source: "qrc:/images/robot_head.png"
                sourceSize.width: 30
                sourceSize.height: 30
                anchors.verticalCenter: parent.verticalCenter
                x: 5
            }
            Text {
                id: headText
                x: 5
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: headImage.right
                anchors.leftMargin: 5
            }
        }
    }

    Rectangle {
        id: mainMenu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: topBar.bottom
        anchors.bottom: bottomMenu.top
        anchors.margins: 5
        height: 50
        color: defaultColor
        border.width: 2
        radius: 5

        ServoControlWindow {
            id: servoControlWindow
            anchors.fill: parent
            visible: syncCommandsButton.selected
            enabled: syncCommandsButton.selected
        }

        ManualDriveWindow {
            id: manualDriveWindow
            anchors.fill: parent
            visible: manualDriveCommandsButton.selected
            enabled: manualDriveCommandsButton.selected
        }

        FaceImageSelectorWindow {
            id: faceImageSelectorWindow
            anchors.fill: parent
            visible: faceImageCommandsButton.selected
            enabled: faceImageCommandsButton.selected
        }

        RowLayout {
            anchors.fill: parent
            visible: manualAudioCommandsButton.selected
            enabled: manualAudioCommandsButton.selected
            Rectangle {
                id: audioButton1
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("1")
                    elide: Text.ElideMiddle
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        audioButton2.selected = false
                        audioButton1.selected = true
                        btController.sendMessageAll(btCommands.audio1Command())
                    }
                }
            }
            Rectangle {
                id: audioButton2
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("2")
                    elide: Text.ElideMiddle
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        audioButton1.selected = false
                        audioButton2.selected = true
                        btController.sendMessageAll(btCommands.audio2Command())
                    }
                }
            }
        }
    }

    Rectangle {
        id: bottomMenu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        height: 100
        color: defaultColor
        border.width: 2
        radius: 5

        GridLayout {
            anchors.fill: parent
            anchors.margins: 5
            columns: 3
            //spacing: 5

            Rectangle {
                id: syncCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: true
                Label {
                    text: qsTr("Servo movement")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        manualDriveCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        cameraViewCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        syncCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: advancedCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Advanced")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        cameraViewCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        advancedCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: manualDriveCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Manual drive")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        cameraViewCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        manualDriveCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: faceImageCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Face image")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        cameraViewCommandsButton.selected = false
                        faceImageCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: manualAudioCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Manual audio")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        cameraViewCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        manualAudioCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: cameraViewCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Camera view")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        cameraViewCommandsButton.selected = true
                    }
                }
            }
        }
    }

}













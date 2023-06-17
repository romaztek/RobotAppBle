import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    id: win

    property var heads: []
    property int head_count: 0
    property var current_head: []

    MouseArea {
        anchors.fill: parent
    }

    function create_robot_head(index, name) {
        var newHead

        if (headsLayout.children.length > 0) {
            newHead = headDelegate.createObject(headsLayout, {
                                                    "id": "head_" + head_count.toString(
                                                              ),
                                                    "anchors.left": headsLayout.children[headsLayout.children.length - 1].right,
                                                    "anchors.leftMargin": 5,
                                                    "text": name,
                                                    "idx": index
                                                })
        } else {
            newHead = headDelegate.createObject(headsLayout, {
                                                    "id": "head_" + head_count.toString(
                                                              ),
                                                    "x": 5,
                                                    "text": name,
                                                    "idx": index
                                                })
        }

        newHead.setChecked(true)

        heads.push(newHead)
        head_count++
        return newHead
    }

    function get_robot_head_name(idx) {
        if (idx < 0 || idx > headsLayout.children.length - 1)
            return ""
        var name = headsLayout.children[idx].text
    }

    Component.onCompleted: {
//        create_robot_head(0, 'Кукла')
//        create_robot_head(1, 'Собака')
    }

    Connections {
        target: btController
        function onDeviceConnected(index, name) {
            create_robot_head(index, name)
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
        radius: 0
        Item {
            id: headsLayout
            anchors.fill: parent
        }
    }

    Component {
        id: headDelegate
        Rectangle {
            id: headDelegateRect
            color: checked ? "lime" : "white"
            border.color: "black"
            border.width: 2
            radius: 0
            width: headText.width + headImage.width + 15
            height: 40
            y: 5
            property alias text: headText.text
            property bool checked: false
            property int idx: -1

            function setChecked(val) {
                checked = val
            }

            onCheckedChanged: {
                if(checked) {
                    if(current_head.length === 0) {
                        current_head.push(idx)
                    } else {
                        if(!(current_head.indexOf(idx) >= 0)) {
                            current_head.push(idx)
                        }
                    }
                } else {
                    var idx_index = current_head.indexOf(idx);
                    if(idx_index >= 0) {
                        if (idx_index !== -1) {
                          current_head.splice(idx_index, 1);
                        }
                    }
                }
                console.log("new current_head: " + current_head)
            }

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
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    parent.checked = !parent.checked
//                    var this_i
//                    for (var i = 0; i < headsLayout.children.length; i++) {
//                        headsLayout.children[i].checked = false
//                        if (headsLayout.children[i] === parent) {
//                            this_i = i
//                        }
//                    }
//                    current_head = this_i
//                    console.log(current_head)
                }
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
        radius: 0

        ServoControlWindow {
            id: servoControlWindow
            anchors.fill: parent
            visible: syncCommandsButton.selected
            enabled: syncCommandsButton.selected
        }

        ManualDriveWindow {
            id: manualDriveWindow
            visible: manualDriveCommandsButton.selected
            enabled: manualDriveCommandsButton.selected
        }

        FaceImageSelectorWindow {
            id: faceImageSelectorWindow
            anchors.fill: parent
            visible: faceImageCommandsButton.selected
            enabled: faceImageCommandsButton.selected
        }

        AudioSelectorWindow {
            anchors.fill: parent
            visible: manualAudioCommandsButton.selected
            enabled: manualAudioCommandsButton.selected

            onVisibleChanged: {
                setVolumeSliderFocus()
            }
        }

        Item {
            anchors.fill: parent
            visible: advancedCommandsButton.selected
            enabled: advancedCommandsButton.selected

            Connections {
                target: btController
                function onRecognitionMsgGot(msg) {

                    switch (msg) {
                    case "O":
                        recognitedText.text = "Оранжевый"
                        break
                    case "G":
                        recognitedText.text = "Зеленый"
                        btController.sendMessageAll("L")
                        break
                    case "B":
                        recognitedText.text = "Синий"
                        break
                    default:
                        recognitedText.text = "Неизвестно"
                        break
                    }
                }
            }

            Label {
                id: recognitedText
                anchors.centerIn: parent
                text: "Неизвестно"
            }
        }

        ChokerWindow {
            anchors.fill: parent
            visible: chokerCommandsButton.selected
            enabled: chokerCommandsButton.selected
        }
    }

    Item {
        id: bottomMenu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        height: 100

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
                radius: 0
                color: selected ? highlightColor : defaultColor
                property bool selected: true
                Label {
                    text: qsTr("Servo movement")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                    color: parent.selected ? defaultColor : highlightColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        manualDriveCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        chokerCommandsButton.selected = false
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
                radius: 0
                color: selected ? highlightColor : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Advanced")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                    color: parent.selected ? defaultColor : highlightColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        chokerCommandsButton.selected = false
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
                radius: 0
                color: selected ? highlightColor : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Manual drive")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                    color: parent.selected ? defaultColor : highlightColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        chokerCommandsButton.selected = false
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
                radius: 0
                color: selected ? highlightColor : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Face image")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                    color: parent.selected ? defaultColor : highlightColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        chokerCommandsButton.selected = false
                        faceImageCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: manualAudioCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 0
                color: selected ? highlightColor : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Manual audio")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                    color: parent.selected ? defaultColor : highlightColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        chokerCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        manualAudioCommandsButton.selected = true
                    }
                }
            }

            Rectangle {
                id: chokerCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 0
                color: selected ? highlightColor : defaultColor
                property bool selected: false
                Label {
                    text: qsTr("Choker")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                    color: parent.selected ? defaultColor : highlightColor
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = false
                        advancedCommandsButton.selected = false
                        manualDriveCommandsButton.selected = false
                        manualAudioCommandsButton.selected = false
                        faceImageCommandsButton.selected = false
                        chokerCommandsButton.selected = true
                    }
                }
            }
        }
    }
}

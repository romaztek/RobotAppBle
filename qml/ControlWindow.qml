import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3

Rectangle {
    id: win

    color: !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? "black" : "white"

    property var heads: []
    property int head_count: 0
    property var current_head: []

    MouseArea {
        anchors.fill: parent
    }

    function create_robot_head(address, name) {
        var newHead

        if (headsLayout.children.length > 0) {
            newHead = headDelegate.createObject(headsLayout, {
                                                    "id": "head_" + head_count.toString(
                                                              ),
                                                    "anchors.left": headsLayout.children[headsLayout.children.length - 1].right,
                                                    "anchors.leftMargin": 5,
                                                    "deviceName": name,
                                                    "deviceAddress": address
                                                })
        } else {
            newHead = headDelegate.createObject(headsLayout, {
                                                    "id": "head_" + head_count.toString(
                                                              ),
                                                    "x": 5,
                                                    "deviceName": name,
                                                    "deviceAddress": address
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
        create_robot_head('1:1:1:1', 'Кукла')
        //        create_robot_head(1, 'Собака')
    }

    Connections {
        target: btController
        onDeviceConnected: {
            create_robot_head(address, name)
        }
    }

    ToolBar {
        id: topBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        height: 50

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
            radius: 4
            width: headText.width + headImage.width + 15
            height: 40
            y: 5
            property bool checked: false
            property int idx: -1
            property string deviceName: ""
            property string deviceAddress: ""

            function setChecked(val) {
                checked = val
            }

            onCheckedChanged: {
                if(checked) {
                    if(current_head.length === 0) {
                        current_head.push(deviceAddress)
                    } else {
                        if(!(current_head.indexOf(deviceAddress) >= 0)) {
                            current_head.push(deviceAddress)
                        }
                    }
                } else {
                    var addr_index = current_head.indexOf(deviceAddress);
                    if(addr_index >= 0) {
                        if (addr_index !== -1) {
                            current_head.splice(addr_index, 1);
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
                text: headDelegateRect.deviceName + "\n" + headDelegateRect.deviceAddress
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
        color: defaultBackgroundColor
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
                onRecognitionMsgGot: {
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

        Item {
            anchors.fill: parent
            visible: dalnomerCommandsButton.selected
            enabled: dalnomerCommandsButton.selected

            Connections {
                target: btController
                function onDalnomerValuesGot(side, value) {

                    switch (side) {
                    case "L":
                        rangeTextLeft.dalnomerValue = value
                        break
                    case "F":
                        rangeTextFront.dalnomerValue = value
//                        btController.sendMessageAll("L")
                        break
                    case "R":
                        rangeTextRight.dalnomerValue = value
                        break
                    default:
                        break
                    }
                }
            }

            GridLayout {
                columns: 3
                rowSpacing: 5
                columnSpacing: 5
                anchors.fill: parent
                anchors.margins: 5
                Button {
                    id: dalnomerEnabler
                    Layout.columnSpan: 3
                    Layout.preferredHeight: 50
                    Layout.fillWidth: true
                    property bool dalnomerEnabled: false
                    text: qsTr("Dalnomer: ") + (dalnomerEnabled ? qsTr("ON") : qsTr("OFF"))
                    background:
                        Rectangle {
                        implicitWidth: 100
                        implicitHeight: 40
                        color: dalnomerEnabler.dalnomerEnabled ? "lime" : "red"
                        border.width: 1
                        radius: 4
                    }
                    onPressed: {
                        dalnomerEnabled = !dalnomerEnabled
                        btController.setDalnomerState(dalnomerEnabled)
                    }
                }

                Label {
                    id: rangeTextLeft
                    property int dalnomerValue: 0
                    text: "LEFT\n" + dalnomerValue.toString()
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    id: rangeTextFront
                    property int dalnomerValue: 0
                    text: "FRONT\n" + dalnomerValue.toString()
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    id: rangeTextRight
                    property int dalnomerValue: 0
                    text: "RIGHT\n" + dalnomerValue.toString()
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }

        Item {
            anchors.fill: parent
            visible: stagesCommandsButton.selected
            enabled: stagesCommandsButton.selected
            ColumnLayout {
                CheckBox {
                    text: qsTr("Stage:") + " 1"
                    Layout.fillWidth: true
                    height: 50
                }
                CheckBox {
                    text: qsTr("Stage:") + " 2"
                    Layout.fillWidth: true
                    height: 50
                }
                CheckBox {
                    text: qsTr("Stage:") + " 3"
                    Layout.fillWidth: true
                    height: 50
                }
            }
        }
    }

    ToolBar {
        id: bottomMenu
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 0
        height: 100

        GridLayout {
            anchors.fill: parent
            anchors.margins: 5
            columns: 4

            //spacing: 5
            ToolButton {
                id: syncCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Servo movement")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: true
                onClicked: {
                    manualDriveCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    syncCommandsButton.selected = true
                }
            }
            ToolButton {
                id: advancedCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Advanced")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    manualDriveCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    advancedCommandsButton.selected = true
                }
            }
            ToolButton {
                id: manualDriveCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Manual drive")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    manualDriveCommandsButton.selected = true
                }
            }
            ToolButton {
                id: faceImageCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Face image")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualDriveCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    faceImageCommandsButton.selected = true
                }
            }
            ToolButton {
                id: manualAudioCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Manual audio")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualDriveCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    manualAudioCommandsButton.selected = true
                }
            }
            ToolButton {
                id: chokerCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Choker")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualDriveCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    chokerCommandsButton.selected = true
                }
            }
            ToolButton {
                id: dalnomerCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Ranger")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualDriveCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    stagesCommandsButton.selected = false
                    dalnomerCommandsButton.selected = true
                }
            }
            ToolButton {
                id: stagesCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                text: qsTr("Stages")
                Material.theme : !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                property bool selected: false
                onClicked: {
                    syncCommandsButton.selected = false
                    advancedCommandsButton.selected = false
                    manualDriveCommandsButton.selected = false
                    manualAudioCommandsButton.selected = false
                    faceImageCommandsButton.selected = false
                    chokerCommandsButton.selected = false
                    dalnomerCommandsButton.selected = false
                    stagesCommandsButton.selected = true
                }
            }

        }
    }
}

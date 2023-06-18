import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    Component.onCompleted: {
        btController.init()
    }

    Connections {
        target: btController
        onNewDeviceFound: {
            var _addr = device.split("#")[0]
            var _name = device.split("#")[1]

            console.log(_addr + " " + _name)
            deviceModel.append({
                                   "address": _addr,
                                   "name": _name,
                                   "checked": false
                               })
        }
    }

    Label {
        id: selectPuppetsLabel
        text: qsTr("Select puppets (one or more):")
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
        height: filterNameCheckBox.height
        verticalAlignment: Qt.AlignVCenter
        font.bold: true
    }

    Button {
        id: skipButton
        text: qsTr("Skip")
        anchors.top: selectPuppetsLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 5
        onClicked: {
            console.log("aaaa")
            destroyConnectWindow()
            createControlWindow()
        }
    }

    CheckBox {
        id: filterNameCheckBox
        text: qsTr("Filter 'Puppet*'")
        checked: true
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 5
    }

    ListModel {
        id: deviceModel
    }

    ListView {
        id: deviceListView
        model: deviceModel
        delegate: deviceDelegate
        spacing: 0
        anchors.top: skipButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: connectButton.top
        anchors.margins: 5
        clip: true
    }

    Component {
        id: deviceDelegate
        Item {
            width: parent.width
            height: filterNameCheckBox.checked && !name.startsWith(
                        "Puppet") ? 0 : 55

            Rectangle {
                width: parent.width
                height: filterNameCheckBox.checked && !name.startsWith(
                            "Puppet") ? 0 : 50
                color: checked ? "lime" : defaultColor
                border.color: "black"
                border.width: 2
                radius: 5
            }

            ColumnLayout {
                x: 5
                visible: filterNameCheckBox.checked && !name.startsWith(
                             "Puppet") ? false : true

                Text {
                    id: nameField
                    text: name
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                Text {
                    id: addressField
                    text: address
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    checked = !checked
                }
            }
        }
    }

    Rectangle {
        id: connectButton
        height: 50
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        border.width: 2
        border.color: connectButtonMouseArea.pressed ? "white" : "black"
        color: connectButtonMouseArea.pressed ? "black" : "white"

        ColorAnimation {
            from: "white"
            to: "black"
            duration: 200
        }
        Text {
            anchors.centerIn: parent
            text: qsTr("Connect")
            color: connectButtonMouseArea.pressed ? "white" : "black"
            font.bold: true
        }
        MouseArea {
            id: connectButtonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {

            }
            onReleased: {

            }
            onClicked: {
                createLoadingWindow()
                var checked_count = 0
                var indexes = []
                var device_indexes = []

                for (var i = 0; i < deviceModel.count; i++) {
                    if (deviceModel.get(i).checked) {
                        indexes.push(i)
                        device_indexes.push(checked_count)
                        checked_count++
                    }
                }

                btController.connectToDevices(indexes, device_indexes)
            }
        }
    }
}

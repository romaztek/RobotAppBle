import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    Component.onCompleted: {
        btController.init()
    }

    Connections{
        target: btController
        onNewDeviceFound: {
            var _addr = device.split("#")[0]
            var _name = device.split("#")[1]

            console.log(_addr + " " + _name)
            deviceModel.append({"address": _addr, "name": _name})
        }
    }

    ListModel {
        id: deviceModel
    }

    ListView {
        id: deviceListView
        model: deviceModel
        delegate: deviceDelegate
        spacing: 5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: connectButton.top
        anchors.margins: 5
    }

    Component {
        id: deviceDelegate
        Rectangle {
            width: parent.width - 10
            height: 50
            x: 5
            border.color: "black"
            border.width: 2
            radius: 5
            color: deviceListView.currentIndex == index ? "lime" : "lightsteelblue"

            ColumnLayout {
                x: 5
                Text { id: nameField; text: name }
                Text { id: addressField; text: address }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: deviceListView.currentIndex = index
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
        border.color: "black"
        border.width: 2
        radius: 5
        color: "lightsteelblue"
        Text {
            anchors.centerIn: parent
            text: qsTr("Connect")
        }
        MouseArea {
            id: connectButtonMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onPressed: {
                connectButton.color = "lime"
            }
            onReleased: {
                connectButton.color = "lightsteelblue"
            }
            onClicked: {
                btController.connectTo(deviceListView.currentIndex)
            }
        }
    }
}

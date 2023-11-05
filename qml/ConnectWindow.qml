import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3

Rectangle {
    color: !(mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? "black" : "white"

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
                                   "is_checked": false
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
        Material.theme : (mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
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
        Material.theme : (mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
    }

    CheckBox {
        id: filterNameCheckBox
        text: qsTr("Filter 'Puppet*'")
        checked: true
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 5
        Material.theme : (mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
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

            CheckBox {
                id: chkbox
                width: parent.width
                height: filterNameCheckBox.checked && !name.startsWith(
                            "Puppet") ? 0 : 50
                visible: filterNameCheckBox.checked && !name.startsWith(
                             "Puppet") ? false : true
                enabled: visible
                Material.theme : (mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark
                text: name + " : " + address
                onCheckStateChanged: {
                    is_checked = checked
                }
            }
        }
    }

    Button {
        id: connectButton
        height: 50
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 5
        text: qsTr("Connect")
        Material.theme : (mySysPalette.windowText.hsvValue<mySysPalette.window.hsvValue) ? Material.Light : Material.Dark

        onClicked: {
            createLoadingWindow()
            var checked_count = 0
            var indexes = []
            var device_indexes = []

            for (var i = 0; i < deviceModel.count; i++) {
                console.log(deviceModel.get(i).is_checked)
                if (deviceModel.get(i).is_checked) {
                    console.log(i)
                    indexes.push(i)
                    device_indexes.push(checked_count)
                    checked_count++
                }
            }

            btController.connectToDevices(indexes, device_indexes)
        }

    }
}

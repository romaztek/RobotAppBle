import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Window {
    id: appwindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    property var connectWindow
    property var controlWindow
    property var loadingWindow

    function createConnectWindow() {
        connectWindow = myConnectWindow.createObject(appwindow, {"id": "connectWindow"});
    }

    function destroyConnectWindow() {
        connectWindow.destroy()
    }

    function createControlWindow() {
        controlWindow = myControlWindow.createObject(appwindow, {"id": "controlWindow"});
    }

    function destroyControlWindow() {
        controlWindow.destroy()
    }

    function createLoadingWindow() {
        loadingWindow = myLoadingWindow.createObject(appwindow, {"id": "loadingWindow"});
    }

    function destroyLoadingWindow() {
        loadingWindow.destroy()
    }

    Component.onCompleted: {
        createConnectWindow()
    }

    Connections {
        target: btController
        onFullyConnected: {
            destroyLoadingWindow()
            destroyConnectWindow()
            createControlWindow()
        }
    }

    Component {
        id: myConnectWindow
        ConnectWindow {
            anchors.fill: parent
        }
    }

    Component {
        id: myControlWindow
        ControlWindow {
            anchors.fill: parent
        }
    }

    Component {
        id: myLoadingWindow
        Item {
            anchors.fill: parent

            Rectangle {
                anchors.fill: parent
                color: "grey"
                opacity: 0.5
            }

            MouseArea {
                anchors.fill: parent
            }

            Rectangle {
                anchors.centerIn: parent
                width: 110
                height: 110
                radius: 55
                border.width: 2
                border.color: "black"
            }

            BusyIndicator {
                anchors.centerIn: parent
                running: true
                width: 100
                height: 100
            }
        }
    }

}













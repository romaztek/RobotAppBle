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

    Component.onCompleted: {
        createConnectWindow()
    }

    Connections {
        target: btController
        onFullyConnected: {
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

}













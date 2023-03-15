import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Window {
    id: appwindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Puppet Controller")

    property var connectWindow
    property var controlWindow
    property var loadingWindow


    function createConnectWindow() {
        connectWindow = myConnectWindow.createObject(appwindow, {"id": "connectWindow"});
    }

    function destroyConnectWindow() {
        if(connectWindow)
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
        if(loadingWindow)
            loadingWindow.destroy()
    }


    Component.onCompleted: {
        createControlWindow()
        createConnectWindow()
    }

    Connections {
        target: btController
        onFullyConnected: {
            destroyLoadingWindow()
            destroyConnectWindow()
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
                width: 60
                height: width
                radius: width/2
                border.width: 2
                border.color: "black"
            }

            BusyIndicator {
                id: myLoadingIndicator
                anchors.centerIn: parent
                running: true
                width: 50
                height: width
                contentItem: Image {
                    visible: myLoadingIndicator.running
                    source: "spinner.png"
                    RotationAnimator on rotation {
                        running: myLoadingIndicator.running
                        loops: Animation.Infinite
                        duration: 2000
                        from: 360 ; to: 0
                    }
                }
            }
        }
    }

}













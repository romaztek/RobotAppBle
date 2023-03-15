import QtQuick 2.7
import QtQuick.Window 2.2
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
        //create_robot_head()
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
        color: "white"
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
            Text {
                id: headText
                x: 5
                text: ""
                anchors.verticalCenter: parent.verticalCenter
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
        color: "white"
        border.width: 2
        radius: 5

        Item {
            anchors.fill: parent
            ScrollView {
                anchors.fill: parent
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AsNeeded
                ColumnLayout {
                    id: syncCommandButtons
                    anchors.fill: parent
                    CommandButton {
                        id: helloButton
                        text: qsTr("Hello")
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                for(var i = 0; i < syncCommandButtons.children.length; i++) {
                                    if(syncCommandButtons.children[i] !== helloButton) {
                                        syncCommandButtons.children[i].selected = false
                                    }
                                    else {
                                        syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                                        if(syncCommandButtons.children[i].selected) {
                                            btController.sendMessageAll("sstop")
                                            btController.sendMessageAll("hello")
                                        } else {
                                            btController.sendMessageAll("sstop")
                                        }
                                    }
                                }
                            }
                        }
                    }
                    CommandButton {
                        id: danceButton
                        text: qsTr("Dance")
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                for(var i = 0; i < syncCommandButtons.children.length; i++) {
                                    if(syncCommandButtons.children[i] !== danceButton) {
                                        syncCommandButtons.children[i].selected = false
                                    }
                                    else {
                                        syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                                        if(syncCommandButtons.children[i].selected) {
                                            btController.sendMessageAll("sstop")
                                            btController.sendMessageAll("dance")
                                        } else {
                                            btController.sendMessageAll("sstop")
                                        }
                                    }
                                }
                            }
                        }
                    }
                    CommandButton {
                        id: byeButton
                        text: qsTr("Bye")
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                for(var i = 0; i < syncCommandButtons.children.length; i++) {
                                    if(syncCommandButtons.children[i] !== byeButton) {
                                        syncCommandButtons.children[i].selected = false
                                    }
                                    else {
                                        syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                                        if(syncCommandButtons.children[i].selected) {
                                            btController.sendMessageAll("sstop")
                                            btController.sendMessageAll("bye")
                                        } else {
                                            btController.sendMessageAll("sstop")
                                        }
                                    }
                                }
                            }
                        }
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
        height: 50
        color: "white"
        border.width: 2
        radius: 5

        RowLayout {
            anchors.fill: parent
            anchors.margins: 5
            spacing: 5

            Rectangle {
                id: syncCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : "white"
                property bool selected: true
                Label {
                    text: qsTr("Sync commands")
                    elide: Text.ElideLeft
                    wrapMode: Text.WordWrap
                    x: 5
                    verticalAlignment: Qt.AlignVCenter
                    height: parent.height
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        syncCommandsButton.selected = true
                        advancedCommandsButton.selected = false
                    }
                }
            }

            Rectangle {
                id: advancedCommandsButton
                Layout.fillWidth: true
                Layout.fillHeight: true
                border.width: 2
                radius: 5
                color: selected ? "lime" : "white"
                property bool selected: false
                Label {
                    text: qsTr("Advanced commands")
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
                        advancedCommandsButton.selected = true
                    }
                }
            }

        }
    }

    //    ScrollView {
    //        anchors.fill: parent
    //        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    //        ScrollBar.vertical.policy: ScrollBar.AsNeeded
    //        ColumnLayout {
    //            spacing: 5
    //            anchors.top: parent.top
    //            anchors.margins: 5
    //            width: win.width - 10
    //            x: 5
    //            Button {
    //                Layout.fillWidth: true
    //                Layout.maximumHeight: 50
    //                text: "YES"
    //                onClicked: {
    //                    btController.sendMessageAll("YES")
    //                }
    //            }
    //            Button {
    //                Layout.fillWidth: true
    //                Layout.maximumHeight: 50
    //                text: "NO"
    //                onClicked: {
    //                    btController.sendMessageAll("NO")
    //                }
    //            }
    //            Item {
    //                Layout.fillWidth: true
    //                Layout.maximumHeight: 50
    //                Layout.minimumHeight: 50
    //            }
    //            Row {
    //                id: sendTextRow
    //                Layout.fillWidth: true
    //                Layout.maximumHeight: 50
    //                spacing: 5

    //                TextInput {
    //                    id: sendText
    //                    text: "Hi"
    //                    width: parent.width/2
    //                    height: 50
    //                }
    //                Button {
    //                    width: parent.width/2
    //                    height: 50
    //                    text: "SEND"
    //                    onClicked: {
    //                        if(sendText.text.length > 0)
    //                            btController.sendMessageAll(sendText.text)
    //                    }
    //                }
    //            }
    //        }
    //    }

}













import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent
    anchors.margins: 5
    GridLayout {
        width: parent.width
        anchors.margins: 5
        columns: 3

        Rectangle {
            id: imageButton1
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            border.width: 2
            radius: 5
            color: selected ? "lime" : defaultColor
            property bool selected: true
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
                    imageButton2.selected = false
                    imageButton3.selected = false
                    imageButton1.selected = true
                    btController.sendMessage(btCommands.faceImage1Command(), current_head)
                }
            }
        }
        Rectangle {
            id: imageButton2
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
                    imageButton1.selected = false
                    imageButton3.selected = false
                    imageButton2.selected = true
                    btController.sendMessage(btCommands.faceImage2Command(), current_head)
                }
            }
        }
        Rectangle {
            id: imageButton3
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            border.width: 2
            radius: 5
            color: selected ? "lime" : defaultColor
            property bool selected: false
            Label {
                text: qsTr("3")
                elide: Text.ElideMiddle
                wrapMode: Text.WordWrap
                x: 5
                verticalAlignment: Qt.AlignVCenter
                height: parent.height
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    imageButton1.selected = false
                    imageButton2.selected = false
                    imageButton3.selected = true
                    btController.sendMessage(btCommands.faceImage3Command(), current_head)
                }
            }
        }
    }
}

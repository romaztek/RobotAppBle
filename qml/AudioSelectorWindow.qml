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
            id: audioButton1
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
                    audioButton2.selected = false
                    audioButton3.selected = false
                    audioButton1.selected = true
                    btController.sendMessageAll(btCommands.audio1Command())
                }
            }
        }
        Rectangle {
            id: audioButton2
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
                    audioButton1.selected = false
                    audioButton3.selected = false
                    audioButton2.selected = true
                    btController.sendMessageAll(btCommands.audio2Command())
                }
            }
        }
        Rectangle {
            id: audioButton3
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
                    audioButton1.selected = false
                    audioButton2.selected = false
                    audioButton3.selected = true
                    btController.sendMessageAll(btCommands.audio3Command())
                }
            }
        }

    }

}

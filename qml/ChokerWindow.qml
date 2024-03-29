import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent
    anchors.margins: 5

    GridLayout {
        id: chokerButtonsGrid
        width: parent.width
        anchors.margins: 5
        columns: 3

        Rectangle {
            id: chokerButton1
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            border.width: 2
            radius: 5
            color: selected ? "lime" : defaultBackgroundColor
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
                    chokerButton2.selected = false
                    chokerButton3.selected = false
                    chokerButton1.selected = true
                    btController.sendMessage("z", current_head)
                }
            }
        }
        Rectangle {
            id: chokerButton2
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            border.width: 2
            radius: 5
            color: selected ? "lime" : defaultBackgroundColor
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
                    chokerButton1.selected = false
                    chokerButton3.selected = false
                    chokerButton2.selected = true
                    btController.sendMessage("x", current_head)
                }
            }
        }
        Rectangle {
            id: chokerButton3
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            border.width: 2
            radius: 5
            color: selected ? "lime" : defaultBackgroundColor
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
                    chokerButton1.selected = false
                    chokerButton2.selected = false
                    chokerButton3.selected = true
                    btController.sendMessage("c", current_head)
                }
            }
        }
    }
}

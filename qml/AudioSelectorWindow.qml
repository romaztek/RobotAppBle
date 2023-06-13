import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent
    anchors.margins: 5

    function setVolumeSliderFocus() {
        volumeSlider.focus = true
    }

    GridLayout {
        id: audioButtonsGrid
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
                    btController.sendMessage(btCommands.audio1Command(),
                                             current_head)
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
                    btController.sendMessage(btCommands.audio2Command(),
                                             current_head)
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
                    btController.sendMessage(btCommands.audio3Command(),
                                             current_head)
                }
            }
        }
    }

    Slider {
        id: volumeSlider
        width: audioButtonsGrid.width
        anchors.top: audioButtonsGrid.bottom
        anchors.topMargin: 5
        value: 3
        from: 1
        to: 5
        stepSize: 1
        focus: true
        onValueChanged: {
            switch (value) {
            case 1:
                btController.sendMessage("v", current_head)
                break
            case 2:
                btController.sendMessage("b", current_head)
                break
            case 3:
                btController.sendMessage("n", current_head)
                break
            case 4:
                btController.sendMessage("m", current_head)
                break
            case 5:
                btController.sendMessage("<", current_head)
                break
            default:
                break
            }
        }
        Keys.onPressed: {
            if (event.key === Qt.Key_VolumeUp) {
                volumeSlider.increase()
                event.accepted = true
            } else if (event.key === Qt.Key_VolumeDown) {
                volumeSlider.decrease()
                event.accepted = true
            }
        }
    }
    Label {
        anchors.top: volumeSlider.bottom
        anchors.topMargin: 5
        text: volumeSlider.value
        anchors.horizontalCenter: parent.horizontalCenter
    }
}

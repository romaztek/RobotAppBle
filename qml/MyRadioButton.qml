import QtQuick 2.7
import QtQuick.Controls 2.2

Rectangle {
    id: myToolButton
    border.width: 2
    radius: itemRadius
    color: selected ? "lime" : defaultColor

    property bool selected: false
    property alias text: myToolRadioButton.text
    property alias checked: myToolRadioButton.checked
    property ButtonGroup buttonGroup
    signal clicked

    RadioButton {
        id: myToolRadioButton
        text: "None"
        height: parent.height
        ButtonGroup.group: buttonGroup

        contentItem: Text {
            text: myToolRadioButton.text
            font: myToolRadioButton.font
            elide: Text.ElideMiddle
            wrapMode: Text.WordWrap
            verticalAlignment: Qt.AlignVCenter
            leftPadding: myToolRadioButton.indicator.width + myToolRadioButton.spacing
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            myToolButton.clicked()
            myToolRadioButton.checked = true
        }
    }
}

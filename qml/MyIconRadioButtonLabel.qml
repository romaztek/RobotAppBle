import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    id: btn
    border.width: 2
    border.color: "black"
    radius: height/4
    color: labelBackgroundColor

    property alias image: btnImage.source
    property alias text: btnText.text
    property alias radioButton: btnRadioButton

    RowLayout {
        spacing: 5
        anchors.fill: parent
        Image {
            id: btnImage
            Layout.maximumWidth: 40
            Layout.maximumHeight: 40
            Layout.margins: 5
            fillMode: Image.PreserveAspectFit
            mipmap: true
            smooth: true
            antialiasing: true
        }
        Text {
            id: btnText
            font.pointSize: 14
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            elide: Qt.ElideRight
            text: qsTr("NULL")
        }
        RadioButton {
            id: btnRadioButton
            Layout.minimumWidth: 40
            Layout.minimumHeight: 40
            Layout.maximumWidth: 40
            Layout.maximumHeight: 40
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: btnRadioButton.checked = true
    }
}

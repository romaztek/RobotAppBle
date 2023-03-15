import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id: cmdButton
    Layout.fillWidth: true
    Layout.preferredHeight: 50
    border.width: 2
    radius: 5
    color: selected ? Qt.lighter("yellow", 1.2) : "white"
    property alias text: cmdButtonText.text
    property bool selected: false

    Label {
        id: cmdButtonText
        text: ""
        elide: Text.ElideLeft
        wrapMode: Text.WordWrap
        x: 5
        verticalAlignment: Qt.AlignVCenter
        height: parent.height
    }

    BusyIndicator {
        id: cmdButtonLoadingIndicator
        height: parent.height - 10
        width: height
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        running: selected

        contentItem: Image {
            visible: cmdButtonLoadingIndicator.running
            source: "spinner.png"
            RotationAnimator on rotation {
                running: cmdButtonLoadingIndicator.running
                loops: Animation.Infinite
                duration: 2000
                from: 360 ; to: 0
            }
        }


    }
}

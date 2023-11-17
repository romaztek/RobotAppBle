import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
//import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.3

Rectangle {
    id: cmdButton
    width: scrollView1.width - 10
    height: 50
    x: 5
    border.width: 2
    radius: itemRadius
    color: selected ? highlightColor : defaultColor
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
        color: parent.selected ? defaultColor : highlightColor
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
            source: "qrc:/spinner.png"
            RotationAnimator on rotation {
                running: cmdButtonLoadingIndicator.running
                loops: Animation.Infinite
                duration: 2000
                from: 360 ; to: 0
            }
        }


    }
}

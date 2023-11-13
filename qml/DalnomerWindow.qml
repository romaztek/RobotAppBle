import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent

    Connections {
        target: btController
        function onDalnomerValuesGot(side, value) {

            switch (side) {
            case "L":
                rangeTextLeft.dalnomerValue = value
                break
            case "F":
                rangeTextFront.dalnomerValue = value
                break
            case "R":
                rangeTextRight.dalnomerValue = value
                break
            default:
                break
            }
        }
    }

    GridLayout {
        columns: 3
        rowSpacing: 5
        columnSpacing: 5
        anchors.fill: parent
        anchors.margins: 5
        Button {
            id: dalnomerEnabler
            Layout.columnSpan: 3
            Layout.preferredHeight: 50
            Layout.fillWidth: true
            property bool dalnomerEnabled: false
            text: qsTr("Dalnomer: ") + (dalnomerEnabled ? qsTr("ON") : qsTr("OFF"))
            background:
                Rectangle {
                implicitWidth: 100
                implicitHeight: 40
                color: dalnomerEnabler.dalnomerEnabled ? "lime" : "red"
                border.width: 1
                radius: 4
            }
            onPressed: {
                dalnomerEnabled = !dalnomerEnabled
                btController.setDalnomerState(dalnomerEnabled)
            }
        }

        Label {
            id: rangeTextLeft
            property var dalnomerValue: 0
            text: "LEFT\n" + dalnomerValue.toString()
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: rangeTextFront
            property var dalnomerValue: 0
            text: "FRONT\n" + dalnomerValue.toString()
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            horizontalAlignment: Text.AlignHCenter
        }

        Label {
            id: rangeTextRight
            property var dalnomerValue: 0
            text: "RIGHT\n" + dalnomerValue.toString()
            Layout.fillWidth: true
            Layout.preferredHeight: 50
            horizontalAlignment: Text.AlignHCenter
        }
    }

}

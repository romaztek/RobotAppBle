import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: win

    ColumnLayout {
        anchors.fill: parent
        Button {
            Layout.minimumWidth: parent.width
            Layout.maximumHeight: 50
            text: "YES"
            onClicked: {
                btController.sendMessage("YES")
            }
        }
        Button {
            Layout.minimumWidth: parent.width
            Layout.maximumHeight: 50
            text: "NO"
            onClicked: {
                btController.sendMessage("NO")
            }
        }
    }

}

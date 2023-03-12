import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

Rectangle {
    id: win

    ScrollView {
        anchors.fill: parent
        ColumnLayout {
            spacing: 5
            anchors.top: parent.top
            anchors.margins: 5
            width: win.width - 10
            x: 5
            Button {
                Layout.fillWidth: true
                Layout.maximumHeight: 50
                text: "YES"
                onClicked: {
                    btController.sendMessage("YES")
                }
            }
            Button {
                Layout.fillWidth: true
                Layout.maximumHeight: 50
                text: "NO"
                onClicked: {
                    btController.sendMessage("NO")
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.maximumHeight: 50
                Layout.minimumHeight: 50
            }
            Row {
                id: sendTextRow
                Layout.fillWidth: true
                Layout.maximumHeight: 50
                spacing: 5

                TextInput {
                    id: sendText
                    text: "Hi"
                    width: parent.width/2
                    height: 50
                }
                Button {
                    width: parent.width/2
                    height: 50
                    text: "SEND"
                    onClicked: {
                        if(sendText.text.length > 0)
                            btController.sendMessage(sendText.text)
                    }
                }
            }
        }
    }

}













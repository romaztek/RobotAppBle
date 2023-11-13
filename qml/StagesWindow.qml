import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    anchors.fill: parent

    ColumnLayout {
        id: checkBoxes
        spacing: 5
        anchors.left: parent.left
        anchors.right: parent.right

        Label {
            text: qsTr("Stages") + ":"
            Layout.topMargin: 5
            Layout.leftMargin: 5
        }

        CheckBox {
            Layout.fillWidth: true
            height: 50
            text: qsTr("Stage") + " 1: " + "Перехват радио"
            checked: false
            onCheckedChanged: {
                if(checked) {
                    btController.sendMessage(btCommands.stage1CommandOn(), current_head)
                } else {
                    btController.sendMessage(btCommands.stage1CommandOff(), current_head)
                }
            }
        }
        CheckBox {
            Layout.fillWidth: true
            height: 50
            text: qsTr("Stage") + " 2: " + "Танк подбит"
            checked: false
            onCheckedChanged: {
                if(checked) {
                    btController.sendMessage(btCommands.stage2CommandOn(), current_head)
                } else {
                    btController.sendMessage(btCommands.stage2CommandOff(), current_head)
                }
            }
        }
        CheckBox {
            Layout.fillWidth: true
            height: 50
            text: qsTr("Stage") + " 3: " + "Лабиринт 1"
            checked: false
            onCheckedChanged: {
                if(checked) {
                    btController.sendMessage(btCommands.stage3CommandOn(), current_head)
                } else {
                    btController.sendMessage(btCommands.stage3CommandOff(), current_head)
                }
            }
        }
        CheckBox {
            Layout.fillWidth: true
            height: 50
            text: qsTr("Stage") + " 4: " + "Лабиринт 2"
            checked: false
            onCheckedChanged: {
                if(checked) {
                    btController.sendMessage(btCommands.stage4CommandOn(), current_head)
                } else {
                    btController.sendMessage(btCommands.stage4CommandOff(), current_head)
                }
            }
        }
        CheckBox {
            Layout.fillWidth: true
            height: 50
            text: qsTr("Stage") + " 5: " + "Шлагбаум"
            checked: false
            onCheckedChanged: {
                if(checked) {
                    btController.sendMessage(btCommands.stage5CommandOn(), current_head)
                } else {
                    btController.sendMessage(btCommands.stage5CommandOff(), current_head)
                }
            }
        }
        CheckBox {
            Layout.fillWidth: true
            height: 50
            text: qsTr("Stage") + " 6: " + "Финиш"
            checked: false
            onCheckedChanged: {
                if(checked) {
                    btController.sendMessage(btCommands.stage6CommandOn(), current_head)
                } else {
                    btController.sendMessage(btCommands.stage6CommandOff(), current_head)
                }
            }
        }

        Item {
            Layout.fillWidth: true
            height: 50
        }

        Button {
            Layout.fillWidth: true
            Layout.leftMargin: 5
            Layout.rightMargin: 5
            height: 50
            text: qsTr("Reset")
            onClicked: {
                for(var i = 0; i < checkBoxes.children.length; i++) {
//                    console.log(checkBoxes.children[i].toString().includes("QQuickCheckBox"))

                    if(checkBoxes.children[i].toString().includes("QQuickCheckBox")) {
                        checkBoxes.children[i].checked = false
                    }
                }

            }
        }
    }
}

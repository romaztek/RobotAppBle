import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Controls.Material 2.2

Item {
    anchors.fill: parent

    ScrollView {
        anchors.fill: parent
        anchors.margins: 2
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        clip: true
        Column {
            id: checkBoxes
            spacing: 5
            anchors.fill: parent
            anchors.margins: 5

            Label {
                id: stagesLabel
                text: qsTr("Stages") + ":"
                x: 5
            }

            CheckBox {
                height: 50
                text: qsTr("Stage") + " 1: " + "Наступление"
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
                height: 50
                text: qsTr("Stage") + " 2: " + "Танк обезврежен"
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
                height: 50
                text: qsTr("Stage") + " 3: " + "Связь установлена"
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
                height: 50
                text: qsTr("Stage") + " 4: " + "Прорыв обороны"
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
                height: 50
                text: qsTr("Stage") + " 5: " + "Город освобожден"
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
                height: 50
                text: qsTr("Stage") + " 6: " + "Салют"
                checked: false
                onCheckedChanged: {
                    if(checked) {
                        btController.sendMessage(btCommands.stage6CommandOn(), current_head)
                    } else {
                        btController.sendMessage(btCommands.stage6CommandOff(), current_head)
                    }
                }
            }

            Button {
                x: 5
                height: 50
                text: qsTr("Reset")
                onClicked: {
                    stagesResetDialog.visible = true
                }
            }

        }
    }

    Dialog {
        id: stagesResetDialog
        implicitWidth: 200
        implicitHeight: 100
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        title: qsTr("Stages Reset")

        standardButtons: Dialog.Yes | Dialog.No
        visible: false
        onAccepted: {
            for(var i = 0; i < checkBoxes.children.length; i++) {
                if(checkBoxes.children[i].toString().includes("CheckBox")) {
                    checkBoxes.children[i].checked = false
                }
            }
            btController.sendMessage(btCommands.stageAllCommandOff(), current_head)
        }
        onRejected: {
            console.log("no")
        }
    }
}





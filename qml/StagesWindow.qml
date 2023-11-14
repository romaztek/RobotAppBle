import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import QtQuick.Controls.Material 2.2

Item {
    anchors.fill: parent
    clip: true

    ScrollView {
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        Grid {
            id: checkBoxes
            columns: 2
            spacing: 5
            anchors.fill: parent
            anchors.margins: 5

            Label {
                id: stagesLabel
                text: qsTr("Stages") + ":"
                x: 5
            }

            Item {
                width: stagesLabel.width
                height: stagesLabel.height
            }

            CheckBox {
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
                if(checkBoxes.children[i].toString().includes("QQuickCheckBox")) {
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





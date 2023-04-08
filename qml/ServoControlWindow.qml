import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ScrollView {
    id: scrollView1
    anchors.fill: parent
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AsNeeded

    property string servoStopCommand: btCommands.servoStopCommand()
    property string servoReturnToDefaultCommand: btCommands.servoReturnToDefaultCommand()
    property string servoHelloCommand: btCommands.servoHelloCommand()
    property string servoDanceCommand: btCommands.servoDanceCommand()
    property string servoByeCommand: btCommands.servoByeCommand()

    Column {
        id: syncCommandButtons
        height: parent.height - 10
        y: 5
        spacing: 5
        CommandButton {
            id: returnToDefaultButton
            text: qsTr("Return to default")
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    for(var i = 0; i < syncCommandButtons.children.length; i++) {
                        if(syncCommandButtons.children[i] !== returnToDefaultButton) {
                            syncCommandButtons.children[i].selected = false
                        }
                        else {
                            syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                            if(syncCommandButtons.children[i].selected) {
                                btController.sendMessageAll(servoStopCommand)
                                btController.sendMessageAll(servoReturnToDefaultCommand)
                                syncCommandButtons.children[i].selected = false
                            } else {
                                btController.sendMessageAll(servoStopCommand)
                            }
                        }
                    }
                }
            }
        }
        CommandButton {
            id: helloButton
            text: qsTr("Hello")
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    for(var i = 0; i < syncCommandButtons.children.length; i++) {
                        if(syncCommandButtons.children[i] !== helloButton) {
                            syncCommandButtons.children[i].selected = false
                        }
                        else {
                            syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                            if(syncCommandButtons.children[i].selected) {
                                btController.sendMessageAll(servoStopCommand)
                                btController.sendMessageAll(servoHelloCommand)
                            } else {
                                btController.sendMessageAll(servoStopCommand)
                            }
                        }
                    }
                }
            }
        }
        CommandButton {
            id: danceButton
            text: qsTr("Dance")
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    for(var i = 0; i < syncCommandButtons.children.length; i++) {
                        if(syncCommandButtons.children[i] !== danceButton) {
                            syncCommandButtons.children[i].selected = false
                        }
                        else {
                            syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                            if(syncCommandButtons.children[i].selected) {
                                btController.sendMessageAll(servoStopCommand)
                                btController.sendMessageAll(servoDanceCommand)
                            } else {
                                btController.sendMessageAll(servoStopCommand)
                            }
                        }
                    }
                }
            }
        }
        CommandButton {
            id: byeButton
            text: qsTr("Bye")
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    for(var i = 0; i < syncCommandButtons.children.length; i++) {
                        if(syncCommandButtons.children[i] !== byeButton) {
                            syncCommandButtons.children[i].selected = false
                        }
                        else {
                            syncCommandButtons.children[i].selected = !syncCommandButtons.children[i].selected
                            if(syncCommandButtons.children[i].selected) {
                                btController.sendMessageAll(servoStopCommand)
                                btController.sendMessageAll(servoByeCommand)
                            } else {
                                btController.sendMessageAll(servoStopCommand)
                            }
                        }
                    }
                }
            }
        }


    }
}

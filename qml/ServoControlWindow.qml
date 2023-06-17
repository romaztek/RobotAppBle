import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    id: scrollView1
    anchors.fill: parent
    clip: true
    Rectangle {
        anchors.top: parent.top
        x: 5
        width: parent.width - 10
        height: 2
        color: "black"
        z: 10
    }
    Rectangle {
        anchors.bottom: parent.bottom
        x: 5
        width: parent.width - 10
        height: 2
        color: "black"
        z: 10
    }

    anchors.topMargin: 5
    anchors.bottomMargin: 5

    property string servoStopCommand: btCommands.servoStopCommand()
    property string servoReturnToDefaultCommand: btCommands.servoReturnToDefaultCommand()
    property string servoHelloCommand: btCommands.servoHelloCommand()
    property string servoDanceCommand: btCommands.servoDanceCommand()
    property string servoByeCommand: btCommands.servoByeCommand()

    ScrollView {
        anchors.fill: parent
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded

        Column {
            id: syncCommandButtons
            height: parent.height - 10
            y: 0
            spacing: 5

            Label {
                text: "Общие команды:"
                x: 5
            }

            CommandButton {
                id: headUpButton
                text: qsTr("Head Up")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btController.sendMessage(btCommands.headUpCommand(),
                                                 current_head)
                    }
                }
            }
            CommandButton {
                id: headDownButton
                text: qsTr("Head Down")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btController.sendMessage(btCommands.headDownCommand(),
                                                 current_head)
                    }
                }
            }
            CommandButton {
                id: headLeftButton
                text: qsTr("Head Left")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btController.sendMessage(btCommands.headLeftCommand(),
                                                 current_head)
                    }
                }
            }
            CommandButton {
                id: headRightButton
                text: qsTr("Head Right")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btController.sendMessage(btCommands.headRightCommand(),
                                                 current_head)
                    }
                }
            }
            CommandButton {
                id: headDefaultButton
                text: qsTr("Head Default")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btController.sendMessage(btCommands.headDefaultCommand(
                                                     ), current_head)
                    }
                }
            }

            Label {
                text: "Команды прошлой куклы:"
                x: 5
            }

            CommandButton {
                id: returnToDefaultButton
                text: qsTr("Return to default")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        btController.sendMessage(btCommands.headDefaultCommand(), current_head)
                    }
                }
            }

            CommandButton {
                id: helloButton
                text: qsTr("Hello")
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        for (var i = 0; i < syncCommandButtons.children.length; i++) {

                            if(!syncCommandButtons.children[i].hasOwnProperty("selected")) {
                                continue
                            }

                            if (syncCommandButtons.children[i] !== helloButton) {
                                syncCommandButtons.children[i].selected = false
                            } else {
                                syncCommandButtons.children[i].selected
                                        = !syncCommandButtons.children[i].selected
                                if (syncCommandButtons.children[i].selected) {
                                    btController.sendMessageAll(
                                                servoStopCommand)
                                    btController.sendMessageAll(
                                                servoHelloCommand)
                                } else {
                                    btController.sendMessageAll(
                                                servoStopCommand)
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
                        for (var i = 0; i < syncCommandButtons.children.length; i++) {

                            if(!syncCommandButtons.children[i].hasOwnProperty("selected")) {
                                continue
                            }

                            if (syncCommandButtons.children[i] !== danceButton) {
                                syncCommandButtons.children[i].selected = false
                            } else {
                                syncCommandButtons.children[i].selected
                                        = !syncCommandButtons.children[i].selected
                                if (syncCommandButtons.children[i].selected) {
                                    btController.sendMessageAll(
                                                servoStopCommand)
                                    btController.sendMessageAll(
                                                servoDanceCommand)
                                } else {
                                    btController.sendMessageAll(
                                                servoStopCommand)
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
                        for (var i = 0; i < syncCommandButtons.children.length; i++) {

                            if(!syncCommandButtons.children[i].hasOwnProperty("selected")) {
                                continue
                            }

                            if (syncCommandButtons.children[i] !== byeButton) {
                                syncCommandButtons.children[i].selected = false
                            } else {
                                syncCommandButtons.children[i].selected
                                        = !syncCommandButtons.children[i].selected
                                if (syncCommandButtons.children[i].selected) {
                                    btController.sendMessageAll(
                                                servoStopCommand)
                                    btController.sendMessageAll(servoByeCommand)
                                } else {
                                    btController.sendMessageAll(
                                                servoStopCommand)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

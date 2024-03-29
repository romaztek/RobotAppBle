﻿import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Rectangle {
    id: iconButton
    height: 50
    color: _ma.pressed ? highlightColor : defaultBackgroundColor
    border.color: "#26282a"
    border.width: 2
    //radius: height/4
    radius: 0

    property alias text: _txt.text
    property alias imageSource: _img.source
    property alias fontSize: _txt.font.pointSize
    property bool noImage: false
    property bool centerAlignment: true

    signal clicked()
    signal pressed()
    signal released()
    signal pressAndHold()

    RowLayout {
        id: mainLayout
        spacing: 5
        anchors.margins: 5
        anchors.fill: parent
        Image {
            id: _img
            Layout.maximumWidth: noImage ? 0 : iconButton.height - 10
            Layout.maximumHeight: noImage ? 0 : iconButton.height - 10
            source: "qrc:/images/bt_icon.png"
            fillMode: Image.PreserveAspectFit
            mipmap: true
            smooth: true
            antialiasing: true
        }
        Text {
            id: _txt
            font.pointSize: 14
            text: ""
            Layout.fillWidth: true
            Layout.fillHeight: true
            verticalAlignment: Text.AlignVCenter
            elide: Qt.ElideRight
            color: _ma.pressed ? defaultBackgroundColor : highlightColor
        }
    }

    MouseArea {
        id: _ma
        anchors.fill: parent
        onClicked: iconButton.clicked()
        onPressed: iconButton.pressed()
        onReleased: iconButton.released()
        onPressAndHold: iconButton.pressAndHold()
    }


}

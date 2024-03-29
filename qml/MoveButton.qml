import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import ru.romankartashev.enums 1.0

Rectangle {
    id: btn

    border.width: 2
    border.color: defaultTextColor

    radius: 0

    property alias text: btnText.text
    property alias image: btnImage.source
    property alias mirror: btnImage.mirror
    property alias rotation: btnImage.rotation
    property alias touchEnabled: ma.enabled

    signal pressed()
    signal released()
    signal clicked()

    property bool isPressed: false

    onIsPressedChanged: {
        if(isPressed) {
            btn.color = Qt.lighter(highlightColor, 1.5)
        } else {
            btn.color = highlightColor
        }
    }

    onPressed: {
        isPressed = true
    }

    onReleased: {
        isPressed = false
    }

    color: highlightColor

    Behavior on color {
        ColorAnimation {
            duration: 150
        }
    }

    ColumnLayout {
        anchors.fill: parent
        Image {
            id: btnImage
            property real sizeScale: 2.5
            Layout.preferredWidth: sourceSize.width/sourceSize.height * prefHeight
            Layout.preferredHeight: prefHeight
            Layout.alignment: Qt.AlignHCenter
            mipmap: true
            smooth: true
            antialiasing: true
            property real prefHeight: btn.width > btn.height ? btn.height/sizeScale : btn.width/sizeScale
        }

        Text {
            id: btnText
            Layout.fillWidth: true
            text: "null"
            font.pointSize: 14
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            color: defaultTextColor
        }
    }

    MultiPointTouchArea {
        id: ma
        mouseEnabled: true
        maximumTouchPoints: 2
        anchors.fill: parent

        onPressed: {
            btn.pressed()
            isPressed = true
        }
        onReleased: {
            btn.released()
            isPressed = false
        }
        onCanceled: {
            btn.released()
            isPressed = false
        }
    }

    //    MouseArea {
    //        id: ma
    //        anchors.fill: parent

    //        onPressed: {
    //            btn.pressed()
    //        }
    //        onReleased: {
    //            btn.released()
    //        }
    //        onClicked: btn.clicked()
    //    }

}

import QtQuick 2.0

Image {
    signal clicked()

    id: root
    opacity: marea2.containsMouse? 1.0: 0.7
    width: 24
    height: 24

    Behavior on opacity {
        NumberAnimation {duration: 100}
    }

    MouseArea {
        id: marea2
        anchors.fill: parent
        anchors.margins: -5
        hoverEnabled: true
        onClicked: {
            root.clicked()
        }
    }
}

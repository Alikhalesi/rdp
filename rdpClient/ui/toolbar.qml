import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Shapes 1.12

Window {
    property int windowEdge: 5

    id: root
    width: 360
    height: 30
    visible: true
    x: Screen.desktopAvailableWidth / 2 - width / 2
    y: -height+root.windowEdge
    title: qsTr("Toolbar")
    flags: Qt.ToolTip | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground |Qt.WindowStaysOnTopHint
    color: "#00000000"

    Behavior on y {
        NumberAnimation { duration: 100 }
    }

    Shape {
        anchors.fill: parent
        ShapePath {
            fillGradient: LinearGradient {
                x1: root.width / 2; y1: 0
                x2: root.width / 2; y2: root.height
                GradientStop { position: 0; color: "#0096c7" }
                GradientStop { position: 0.49; color: "#0077b6" }
                GradientStop { position: 0.5; color: "#03045e" }
                GradientStop { position: 1; color: "#023e8a" }
            }

            startX: 0; startY: 0
            PathLine { x: root.width; y: 0 }
            PathLine { x: root.width - 30; y: 30 }
            PathLine { x: 30; y: 30 }
            PathLine { x: 0; y: 0 }
        }
    }

    Timer {
        id: hideTimer
        interval: 1500
        running: false
        repeat: false
        onTriggered: {
            root.y = -root.height+root.windowEdge
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onEntered: {
            if (hideTimer.running)
                hideTimer.stop()
            if (root.y < 0) {
                root.y = 0
            }
        }

        onExited: {
            //delayed hide
            if (!hideTimer.running)
                hideTimer.start()
        }
    }

    ToolbarButton {
        id: imgStartStop
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 30

        CustomBusyIndicator {
            id: indicator
            anchors.fill: parent
            anchors.margins: -3
            running: true
        }

        //        Timer {
        //            id: fakeConn
        //            interval: 3000
        //            running: imgStartStop.state === "connecting"
        //            repeat: false
        //            onTriggered: {
        //                imgStartStop.state = "error"
        //            }
        //        }

        onClicked: {

            if (state === "stopped")
            {
                state = "started" // "connecting"
                console.log('play');
                _clientController.start(txtIp.text)
            }
            else if (state === "started")
            {
                state = "stopped"
                console.log('stop')
                _clientController.stop();
            }
            else if (state === "connecting")
            {
                // do nothing on clicked
            }
            else if (state === "error")
            {
                state = "stopped"
            }
        }

        Component.onCompleted: {
            state = "stopped"
        }


        states: [
            State {
                name: "stopped"
                PropertyChanges {
                    target: imgStartStop
                    source: "qrc:/resources/icons8-play-48.png"
                }
                PropertyChanges {
                    target: indicator
                    visible: false
                }
                PropertyChanges {
                    target: txtIpBg
                    visible: true
                }
            },
            State {
                name: "started"
                PropertyChanges {
                    target: imgStartStop
                    source: "qrc:/resources/icons8-no-entry-48.png"
                }
                PropertyChanges {
                    target: indicator
                    visible: false
                }
                PropertyChanges {
                    target: txtIpBg
                    visible: false
                }
            },
            State {
                name: "connecting"
                PropertyChanges {
                    target: imgStartStop
                    source: ""
                }
                PropertyChanges {
                    target: indicator
                    visible: true
                }
                PropertyChanges {
                    target: txtIpBg
                    visible: false
                }
            },
            State {
                name: "error"
                PropertyChanges {
                    target: imgStartStop
                    source: "qrc:/resources/icons8-error-48.png"
                }
                PropertyChanges {
                    target: indicator
                    visible: false
                }
                PropertyChanges {
                    target: txtIpBg
                    visible: false
                }
            }
        ]
    }

    Rectangle {
        id: txtIpBg
        width: 100
        height: 24
        color: "#77FFFFFF"
        opacity: 1.0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        TextInput {
            id:txtIp
            clip: true
            horizontalAlignment: TextInput.AlignHCenter
            verticalAlignment: TextInput.AlignVCenter

            anchors.fill: parent
            anchors.margins: 3
            anchors.verticalCenter: parent.verticalCenter
            text: "127.0.0.1"
        }
    }

    Text {
        id: txtIpConst
        clip: true
        horizontalAlignment: TextInput.AlignHCenter
        verticalAlignment: TextInput.AlignVCenter

        anchors.fill: txtIpBg
        anchors.margins: 3
        anchors.verticalCenter: txtIpBg.verticalCenter
        text: txtIp.text
        color: "white"
        visible: !txtIpBg.visible
    }

    ToolbarButton {
        id: imgExit
        source: "qrc:/resources/icons8-shutdown-48.png"

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 30

        onClicked: {
            Qt.quit();
            console.log('exit')
        }
    }
}

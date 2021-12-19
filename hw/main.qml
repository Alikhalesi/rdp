import QtQuick
import QtQuick.Controls
import Qt.labs.platform

ApplicationWindow {
    visible: true
    //height:au
    //width:400
    header: ToolBar {
           Flow {
               anchors.fill: parent
               ToolButton {
                   id:btnStart
                   text: qsTr("Start")
                   icon.name: "document-open"
                   onClicked:function(){
                       lblClient.text="WaitForClient"
                        statusIndicator.color="yellow"
                       btnStart.visible=false
                       btnStop.visible=true
                       _serverController.start()

                   }
               }
               ToolButton {
                   id:btnStop
                   visible: false
                   text: qsTr("Stop")
                   icon.name: "document-open"
                   onClicked:function(){
                       lblClient.text="Stopping"

                       _serverController.stop()

                   }
               }
               ToolButton {
                   id:btnStartcmd
                   visible: true
                   text: qsTr("Start cmd")
                   icon.name: "document-open"
                   onClicked:function(){
                       lblClient.text="cmd start"

                       _commandController.start()

                   }
               }



           }
       }



    footer: ToolBar {
        Flow{
        Label
        {
            id:lblClient
            text:qsTr("Stopped")

        }
        Rectangle {
            id:statusIndicator
             width: parent.width<parent.height?parent.width:parent.height
             height: width
             color: "red"
             border.color: "black"
             border.width: 1
             radius: width*0.5

        }
        }



    }
    Connections {
           target: _serverController
           onNewConnection: function(cnnInfo) {
               console.log("Data: " + cnnInfo)
               statusIndicator.color="green"
               lblClient.text = "New Connection: "+cnnInfo
           }

          
           onStopped:function()
           {
               lblClient.text = "Stopped";
               statusIndicator.color="red"
               btnStart.visible=true
               btnStop.visible=false
           }
       }
    Component.onCompleted: {
            //btnStart.clicked()
}

}


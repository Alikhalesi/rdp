import QtQuick
import QtQuick.Controls
import Qt.labs.platform

ApplicationWindow {
    visible: true
    height:600
    width:600
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

    Image
    {
        id:centerImg
        cache: false
        source: "image://_frameImageProvider"
        height:parent.height
        width:parent.width
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
              //  centerImg.source="image://_frameImageProvider"
               console.log("Data: " + cnnInfo)
               statusIndicator.color="green"
               lblClient.text = "New Connection: "+cnnInfo
           }

           onImageChanged:function()
           {
               var oldsrc=centerImg.source;
               centerImg.source="";
               centerImg.source=oldsrc;
               console.log("new image");
           }
           onStopped:function()
           {
               lblClient.text = "Stopped";
               statusIndicator.color="red"
               centerImg.source="";
               btnStart.visible=true
               btnStop.visible=false
           }
       }
    Component.onCompleted: {
            //btnStart.clicked()
}

}


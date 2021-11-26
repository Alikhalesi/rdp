import QtQuick  2.15
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
                   enabled: txtIp.acceptableInput
                   icon.name: "document-open"
                   onClicked:function(){
                       lblClient.text="Connecting...";
                        statusIndicator.color="yellow";
                       btnStart.visible=false;
                       btnStop.visible=true;

                       _clientController.start(txtIp.text)

                   }
               }
               ToolButton {
                   id:btnStop
                   visible: false
                   text: qsTr("Stop")
                   icon.name: "document-open"
                   onClicked:function(){
                       lblClient.text="Disconnecting..."

                       _clientController.stop()

                   }
               }

               TextField
               {
                   id:txtIp
                   placeholderText: qsTr("127.0.0.1:8084")
                   validator: RegularExpressionValidator { regularExpression: /\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?):\d{1,5}\b/ }
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
            text:qsTr("Disconnected")

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
        target: _clientController
        onConnected: function() {
             centerImg.source="image://_frameImageProvider"
            lblClient.text = "Connected"
             statusIndicator.color="green"
            txtIp.visible=false
           }

           onImageChanged:function()
           {
               var oldsrc=centerImg.source;
               centerImg.source="";
               centerImg.source=oldsrc;
               console.log("new image");
           }
           onDisconnected:function()
           {
               lblClient.text = "Disconnected";
               statusIndicator.color="red"
               centerImg.source="";
               txtIp.visible=true
               btnStart.visible=true
               btnStop.visible=false
           }
       }

    MouseArea {
      anchors.fill: parent
      enabled: false
      cursorShape: Qt.BlankCursor
    }

}

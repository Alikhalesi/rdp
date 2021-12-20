import QtQuick  2.15
import QtQuick.Controls
import QtQuick.Controls.Universal 2.3

ApplicationWindow {
    visible: true

   // flags: Qt.FramelessWindowHint | Qt.Window

   // visibility: Qt.
    width:400
    height:200
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
                       lblError.text="";
                       _clientController.start(txtIp.text)
                      //  _commandController.start(txtIp.text)
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
                        _commandController.stop()
                   }
               }

               TextField
               {
                   width: 80

                   id:txtIp
                   placeholderText: qsTr("127.0.0.1")

                   validator: RegularExpressionValidator { regularExpression: /\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b/ }
               }
               ToolButton
               {

                onClicked: Qt.exit(0)
                text: "exit"
               }

           }

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
        Label
        {
           // anchors.right: 0
            id:lblError
            text:qsTr("")

        }



        }



    }
    Connections {
        target: _clientController
        onConnected: function() {
           //  centerImg.source="image://_frameImageProvider"
            lblClient.text = "Connected"
             statusIndicator.color="green"
            txtIp.visible=false
          //  _commandController.start(txtIp.text)
           }

           onImageChanged:function()
           {
              // var oldsrc=centerImg.source;
               centerImg.source="";
               centerImg.source="image://_frameImageProvider";
              // console.log("new image");
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
           onError:function(err)
           {
               lblError.text = err;
               statusIndicator.color="red"
               centerImg.source="";
               txtIp.visible=true
               btnStart.visible=true
               btnStop.visible=false
             // _commandController.stop()
           }
       }

    Connections {
        target: _commandController


           onError:function(err)
           {
               lblError.text = err;
               statusIndicator.color="red"
               centerImg.source="";
               txtIp.visible=true
               btnStart.visible=true
               btnStop.visible=false
              _clientController.stop()
           }
           onConnected: function() {

               lblClient.text = "Connected"
                statusIndicator.color="green"
               txtIp.visible=false

              }


              onDisconnected:function()
              {
                  lblClient.text = "Disconnected";
                  statusIndicator.color="red"

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

#include "p2pChannel.hpp"
//==================================================================================================
void P2PChannel::Start(int port)
{
        Listen(port);

}
//==================================================================================================
void P2PChannel::Stop()
{
    if(client_!=nullptr && client_->state()==QAbstractSocket::ConnectedState)
    {
        client_->close();
        client_=nullptr;

    }
        if(server_!=nullptr)
        {   server_->close();
            server_->deleteLater();
            server_=nullptr;
        }
      //  emit stopped();
}
//==================================================================================================
void P2PChannel::NewFrame(unsigned char *data, unsigned int len)
{
    qDebug()<<"NewFrame:"<<len;
    if(client_!=nullptr &&   client_->state()==QAbstractSocket::ConnectedState)
    {
      //  qDebug()<<"NewFrame\n";
      client_->write((char*)(&len),sizeof(unsigned int));
       client_->waitForBytesWritten();
     auto written= client_->write((char*)(data),len);

     client_->waitForBytesWritten();
    }

}
//==================================================================================================
void P2PChannel::disconnectd()
{
    qDebug()<<"Disconnected ---------------------";
    client_->close();
    client_=nullptr;
    emit clientDisconnected();
}
//==================================================================================================
void P2PChannel::Listen(int port)
{
    if(server_==nullptr)
    {
        server_=new QTcpServer(this);
        connect(server_,SIGNAL(newConnection()),this,SLOT(NewClient()));
    }
if(!server_->isListening())
{
server_->listen(QHostAddress::Any,port);
}


}

//==================================================================================================
void P2PChannel::NewClient()
{


        if(server_->hasPendingConnections())
    {

        client_=server_->nextPendingConnection();
server_->close();
 connect(client_,SIGNAL(disconnected()),this,SLOT(disconnectd()));
   emit clientConnected(client_->localAddress().toString()+":"+QString::number(client_->localPort()));

    }


}

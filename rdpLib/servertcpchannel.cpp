#include "servertcpchannel.h"
//======================================================================================
ServerTcpChannel::ServerTcpChannel(int port, QObject *parent): QObject(parent)
,port_{port}
{


}
//======================================================================================
ServerTcpChannel::ServerTcpChannel(std::vector<std::unique_ptr<IChannelInterceptor>>&& interceptors,int port, QObject *parent): QObject(parent)
,port_{port}
  ,interceptors_{std::move(interceptors)}
{

    if(!interceptors_.empty())
    {
    connect(this,SIGNAL(dataReadyInternal(QVariant)),interceptors_[0].get(),SLOT(dataInput(QVariant)),Qt::DirectConnection);

    connect(interceptors_[0].get(),SIGNAL(dataOutput(QVariant)),this,SIGNAL(dataReady(QVariant)),Qt::DirectConnection);
    }
    else
    {
            connect(this,SIGNAL(dataReadyInternal(QVariant)),this,SIGNAL(dataReady(QVariant)),Qt::DirectConnection);
    }
}
//======================================================================================
void ServerTcpChannel::Start()
{




    if(server_==nullptr)
    {




        server_=new QTcpServer(this);



        //chain


        connect(server_,SIGNAL(newConnection()),this,SLOT(NewClient()));
    }
if(!server_->isListening())
{
server_->listen(QHostAddress::Any,port_);
}
}
//======================================================================================
void ServerTcpChannel::Stop()
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
}
//==================================================================================================
void ServerTcpChannel::ClientDisconnectd()
{
    qDebug()<<"Disconnected ---------------------";
    client_->close();
    client_=nullptr;
    emit clientDisconnected();
}
//==================================================================================================
void ServerTcpChannel::readyRead()
{
    qDebug()<<"readyRead ---------------------";
    auto data=client_->readAll();
    emit dataReadyInternal(data);
}
//==================================================================================================
void ServerTcpChannel::NewClient()
{
qDebug()<<"NewClient ---------------------";

        if(server_->hasPendingConnections())
    {

        client_=server_->nextPendingConnection();
server_->close();
 connect(client_,SIGNAL(disconnected()),this,SLOT(ClientDisconnectd()));
   connect(client_,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);




   emit clientConnected(client_->localAddress().toString()+":"+QString::number(client_->localPort()));

    }


}

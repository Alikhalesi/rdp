#include "ClientTcpChannel.h"

ClientTcpChannel::ClientTcpChannel(QObject *parent): QObject(parent)
{

}
//=====================================================================================
ClientTcpChannel::ClientTcpChannel(std::vector<std::unique_ptr<IChannelInterceptor>>&& interceptors,QObject *parent) : QObject(parent)
  ,interceptors_{std::move(interceptors)}
{


}
//=====================================================================================
void ClientTcpChannel::Start(const QString &ip, int port)
{
     std::lock_guard lock{syncLock_};
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    socket_=new QTcpSocket(this);
    connect(socket_,SIGNAL(connected()),this,SIGNAL(connected()),Qt::DirectConnection);
    connect(socket_,SIGNAL(disconnected()),this,SIGNAL(disconnected()),Qt::DirectConnection);
    connect(socket_,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket_,&QTcpSocket::errorOccurred,this,&ClientTcpChannel::SocketError,Qt::DirectConnection);
    //chain
    if(!interceptors_.empty())
    {
    connect(this,SIGNAL(dataReadyInternal(QVariant)),interceptors_[0].get(),SLOT(dataInput(QVariant)),Qt::DirectConnection);

    connect(interceptors_[0].get(),SIGNAL(dataOutput(QVariant)),this,SIGNAL(dataReady(QVariant)),Qt::DirectConnection);
    }
    else
    {
            connect(this,SIGNAL(dataReadyInternal(QVariant)),this,SIGNAL(dataReady(QVariant)),Qt::DirectConnection);
    }
    //
    ConnectToHost(ip,port);
}
//=====================================================================================
void ClientTcpChannel::ConnectToHost(const QString& ip, int port)
{

  qDebug()<<"ClientTcpChannel::ConnectToHost";
    if(socket_->state()==QAbstractSocket::UnconnectedState)
    {
        socket_->connectToHost(ip,port);
        socket_->waitForConnected();



    }

}
//=====================================================================================
void ClientTcpChannel::Send(const char* data,int len)
{
     int written=socket_->write( data, len);
     socket_->waitForBytesWritten();
     qDebug()<<"sent : "<<len;

     assert(written==len);
}
//=====================================================================================
void ClientTcpChannel::SendAndDelete(std::shared_ptr<char[]> data, int len)
{
    int written=socket_->write( data.get(), len);
    socket_->waitForBytesWritten();
    qDebug()<<"sent : "<<len;

    assert(written==len);
}
//=====================================================================================
void ClientTcpChannel::Disconnect()
{

std::lock_guard lock{syncLock_};
    if(socket_!=nullptr)
    {
    if(socket_->state()==QAbstractSocket::ConnectedState)
    {

        socket_->close();
    socket_->disconnectFromHost();
    }else
    {
         socket_->close();
        emit disconnected();
    }
    socket_->deleteLater();
    socket_=nullptr;
    }
}
//=====================================================================================
void ClientTcpChannel::SocketError(QAbstractSocket::SocketError socketError)
{

  emit Error(QVariant::fromValue(socketError).toString()+":"+socket_->errorString());

}

//=====================================================================================
void ClientTcpChannel::readyRead()
{
auto data=socket_->readAll();

emit dataReadyInternal(data);
}
//=====================================================================================

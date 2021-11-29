#include "clientController.hpp"
//====================================================================================================
ClientController::ClientController(QObject *parent)
    :QObject(parent)
{


}
//====================================================================================================
void ClientController::start(QString ip)
{
    std::vector<std::unique_ptr<IChannelInterceptor>> interceptors{} ;
   interceptors.push_back(std::make_unique<LenValInterceptor>());

    channel_=new ClientTcpChannel( std::move(interceptors),this);
    connect(channel_,SIGNAL(connected()),this,SIGNAL(connected()));
    connect(channel_,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
    connect(channel_,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
    connect(channel_,SIGNAL(dataReady(QVariant)),this,SLOT(newImage(QVariant)));
    connect(channel_,&ClientTcpChannel::Error,this,&ClientController::onError);

    channel_->Start(ip,8084);



}
//====================================================================================================
void ClientController::stop()
{
    if(channel_!=nullptr)
    {

        channel_->Disconnect();
    }
}
//====================================================================================================
void ClientController::newImage(QVariant data)
{
    auto dt=data.toByteArray();
    qDebug()<<"ClientController::newImage: "<<dt.length();
    FrameImageProvider::GetInstance()->SetImageData(std::make_pair((unsigned char*) dt.data(),dt.length()));
    emit imageChanged();
}
//====================================================================================================
void ClientController::onError(const QString &err)
{
    stop();
    emit error(err);
}
//====================================================================================================

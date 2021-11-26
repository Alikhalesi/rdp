#include "clientController.hpp"
//====================================================================================================
ClientController::ClientController(QObject *parent)
    :QObject(parent)
{


}
//====================================================================================================
void ClientController::start(QString ipPort)
{
    std::vector<std::unique_ptr<IChannelInterceptor>> interceptors{} ;
   interceptors.push_back(std::make_unique<LenValInterceptor>());

    channel_=new ClientTcpChannel( std::move(interceptors),this);
    connect(channel_,SIGNAL(connected()),this,SIGNAL(connected()));
    connect(channel_,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
     connect(channel_,SIGNAL(dataReady(QVariant)),this,SLOT(newImage(QVariant)));

    channel_->Start(ipPort.split(":")[0],8084);
    auto ct=new CommandController();
    ct->Start(ipPort.split(":")[0],8085);


}
//====================================================================================================
void ClientController::stop()
{
    channel_->Disconnect();
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

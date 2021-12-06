#include "clientController.hpp"
//====================================================================================================
ClientController::ClientController(QObject *parent)
    :QObject(parent)
{
    uiUpdaterThread_=new QThread(this);
    channelThread_=new QThread(this);

    std::vector<std::unique_ptr<IChannelInterceptor>> interceptors{} ;
   interceptors.push_back(std::make_unique<LenValInterceptor>());

    channel_=new ClientTcpChannel(std::move(interceptors));
    connect(channel_,SIGNAL(connected()),this,SIGNAL(connected()),Qt::QueuedConnection);
    connect(channel_,SIGNAL(disconnected()),this,SIGNAL(disconnected()),Qt::QueuedConnection);
    connect(channel_,&ClientTcpChannel::Error,this,&ClientController::onError,Qt::QueuedConnection);

    connect(channel_,&ClientTcpChannel::dataReady,[](QVariant x)
    {
        auto dt=x.toByteArray();
        auto writer=FrameManager::GetInstance().GetBufferForWrite();
        writer.SetFrameLen(dt.length());
        memcpy(*writer,dt.data(),dt.length());
    });


    connect(uiUpdater::GetInstance(),&uiUpdater::newImage,this,&ClientController::newImage,Qt::BlockingQueuedConnection);

    connect(this,&ClientController::startProcessing,uiUpdater::GetInstance(),&uiUpdater::start,Qt::QueuedConnection);
    connect(this,&ClientController::stopProcessing,uiUpdater::GetInstance(),&uiUpdater::stop,Qt::BlockingQueuedConnection);


    connect(this,&ClientController::startChannelProcessing,channel_,&ClientTcpChannel::Start,Qt::QueuedConnection);
    connect(this,&ClientController::stopProcessing,channel_,&ClientTcpChannel::Disconnect,Qt::BlockingQueuedConnection);

    uiUpdater::GetInstance()->moveToThread(uiUpdaterThread_);
    uiUpdaterThread_->start();

    channel_->moveToThread(channelThread_);
    channelThread_->start();


}
//====================================================================================================
void ClientController::start(QString ip)
{
    emit startChannelProcessing(ip,8084);
    emit startProcessing();
    //channel_->Start(ip,8084);
}
//====================================================================================================
void ClientController::stop()
{
    emit stopProcessing();
}
//====================================================================================================
void ClientController::newImage()
{
    //auto dt=data.toByteArray();
    qDebug()<<"ClientController::newImage: ";
   // FrameImageProvider::GetInstance()->SetImageData(std::make_pair((unsigned char*) dt.data(),dt.length()));
    emit imageChanged();
}
//====================================================================================================
void ClientController::onError(const QString &err)
{
    stop();
    emit error(err);
}
//====================================================================================================

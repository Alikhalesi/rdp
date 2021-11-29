#include "Controller/commandController.hpp"
//====================================================================================
CommandController::CommandController(QObject *parent)
    :QObject(parent)
{
    channel_=new ClientTcpChannel(this);
    channelThread_=new QThread(this);
    connect(channel_,&ClientTcpChannel::connected,this,[this](){
    //send client resulotion
    SendClientResulotion();

    StartCapture();
    },Qt::ConnectionType::QueuedConnection);

    connect(channel_,&ClientTcpChannel::disconnected,this,&CommandController::disconnected,Qt::QueuedConnection);
connect(channel_,&ClientTcpChannel::connected,this,&CommandController::connected,Qt::QueuedConnection);
    connect(this,&CommandController::StartChannel,channel_,&ClientTcpChannel::Start,Qt::QueuedConnection);
    connect(this,&CommandController::SendCommand,channel_,&ClientTcpChannel::SendAndDelete,Qt::QueuedConnection);
       connect(channel_,&ClientTcpChannel::Error,this,&CommandController::onError,Qt::QueuedConnection);
    channelThread_->start();

}
//====================================================================================
void CommandController::start(const QString& ip)
{
    emit StartChannel(ip,8085);
}
//====================================================================================
void CommandController::stop()
{
    channel_->Disconnect();


    StopCapture();
}
//====================================================================================
void CommandController::newCommand(inputCommand cmd)
{
    qDebug()<<"rec-----------"<<(int)cmd.type;
    SendCommandToServer(cmd);
}
//====================================================================================
void CommandController::StartCapture()
{
    connect(&LocalHookManager::GetInstance(),SIGNAL(newCommand(inputCommand)),this,SLOT(newCommand(inputCommand)),Qt::DirectConnection);
    LocalHookManager::GetInstance().Start();
}
//====================================================================================
void CommandController::StopCapture()
{
 LocalHookManager::GetInstance().Stop();
 disconnect(&LocalHookManager::GetInstance(),SIGNAL(newCommand(inputCommand)),this,SLOT(newCommand(inputCommand)));
}
//=================================================================
void CommandController::SendClientResulotion()
{
    RECT r;
    GetClientRect(nullptr,&r);
    InfoCommand infCmd;
    infCmd.res=ScreenResolution{GetSystemMetrics(SM_CYSCREEN),GetSystemMetrics(SM_CXSCREEN)};
    inputCommand cmd;
    cmd.type=commandType::Info;
    cmd.cmd.infoCommand=infCmd;
    SendCommandToServer(cmd);

}
//====================================================================================================
void CommandController::onError(const QString &err)
{
    stop();
    emit error(err);
}
//====================================================================================================
void CommandController::SendCommandToServer(inputCommand cmd)
{
    auto data=new char[sizeof(inputCommand)];
    memcpy(data,&cmd,sizeof(inputCommand));
    std::shared_ptr<char[]> dt{data};
    emit SendCommand(dt,sizeof(inputCommand));
}
//====================================================================================================

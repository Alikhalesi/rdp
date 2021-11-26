#include "Controller/commandController.hpp"
//====================================================================================

//====================================================================================
void CommandController::Start(const QString& ip,int port)
{
    channel_=new ClientTcpChannel(this);
    connect(channel_,&ClientTcpChannel::connected,this,[this](){
    StartCapture();
    });
    channel_->Start(ip,port);

}
//====================================================================================
void CommandController::Stop()
{
    channel_->Disconnect();

    channel_->deleteLater();
    StopCapture();
}
//====================================================================================
void CommandController::newCommand(inputCommand cmd)
{
    qDebug()<<"rec----------------------------------";
    channel_->Send((char*)&cmd,sizeof(inputCommand));

}
//====================================================================================
void CommandController::StartCapture()
{
    connect(&LocalHookManager::GetInstance(),SIGNAL(newCommand(inputCommand)),this,SLOT(newCommand(inputCommand)));
    LocalHookManager::GetInstance().Start();
}
//====================================================================================
void CommandController::StopCapture()
{
 LocalHookManager::GetInstance().Stop();
 disconnect(&LocalHookManager::GetInstance(),SIGNAL(newCommand(inputCommand)),this,SLOT(newCommand(inputCommand)));
}

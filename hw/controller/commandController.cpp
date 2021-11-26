#include "commandController.hpp"
CommandController::CommandController(QObject *parent)
{

}
//==========================================================================================
void CommandController::start()
{

    std::vector<std::unique_ptr<IChannelInterceptor>> interceptors{} ;
      interceptors.push_back(std::make_unique<InputCommandInterceptor>());

       channel_=new ServerTcpChannel( std::move(interceptors),8085,this);
   //    connect(channel_,SIGNAL(connected()),this,SIGNAL(connected()));
   //    connect(channel_,SIGNAL(disconnected()),this,SIGNAL(disconnected()));
        connect(channel_,SIGNAL(dataReady(QVariant)),this,SLOT(newCommand(QVariant)));
channel_->Start();

}
//==========================================================================================
void CommandController::stop()
{

}
//==========================================================================================
void CommandController::newCommand(QVariant val)
{
    inputCommand cmd=val.value<inputCommand>();
    qDebug()<<"cmd received : "<<(int) cmd.type;
    //



}
//==========================================================================================

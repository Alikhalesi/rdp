#include "serverController.hpp"


ServerController::ServerController()
{
    connect(&streamer,SIGNAL(newConnection(QString)),this,SIGNAL(newConnection(QString)), Qt::QueuedConnection);
    connect(&streamer,SIGNAL(stopped()),this,SIGNAL(stopped()),Qt::QueuedConnection);
    connect(this,SIGNAL(startStream()),&streamer,SLOT(Start()),Qt::QueuedConnection);
    connect(this,SIGNAL(stopStream()),&streamer,SLOT(Stop()),Qt::QueuedConnection);

}
//=============================================================
void ServerController::start()
{
    qDebug()<<"ServerController::start\n";
//emit imageChanged();
    //streamer.Start();
    emit startStream();

}
//======================================================================================================
void ServerController::stop()
{
    qDebug()<<"ServerController::stop\n";
    //streamer.Stop();
    emit stopStream();

}
//======================================================================================================

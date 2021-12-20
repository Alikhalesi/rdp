#pragma once
#include <QObject>
#include <QThread>
#include <memory>
#include <Windows.h>
#include <qdebug.h>
#include <QCoreApplication>
#include <QEventLoop>
#include <managers/frameManager.hpp>

class streamerAdapter:public QObject
{
    Q_OBJECT
public:
    virtual ~streamerAdapter()=default;
public slots:

    virtual void Start()=0;
    virtual void StartProcess(QString)=0;
    virtual void Stop()=0;

private:
signals:
  void StopSignal();
  void StartSignal(int port);

  void newConnection(QString ip);

  void SendToClient(unsigned char* data,unsigned int len);


  void stopped();

};

template <class FrameGenerator,class Channel>
class Streamer:public streamerAdapter
{

public:
    Streamer()
        :frameGenerator_{ new FrameGenerator()},
          channel_{ new Channel{}}

    {

      generatorThread=new QThread(this);
        frameGenerator_->moveToThread(generatorThread);
         channelThread=new QThread(this);
        channel_->moveToThread(channelThread);
        this->moveToThread(channelThread);


        connect(this,SIGNAL(StartSignal(int)),channel_.get(),SLOT(Start(int)), Qt::DirectConnection);
        connect(channel_.get(),SIGNAL(clientConnected(QString)),frameGenerator_.get(),SLOT(Start(QString)), Qt::QueuedConnection);

        connect(channel_.get(),SIGNAL(clientDisconnected()),this,SLOT(Stop()), Qt::DirectConnection);
        connect(this,SIGNAL(StopSignal()),frameGenerator_.get(),SLOT(Stop()), Qt::BlockingQueuedConnection);
        connect(this,SIGNAL(StopSignal()),channel_.get(),SLOT(Stop()), Qt::DirectConnection);
        connect(channel_.get(),SIGNAL(clientConnected(QString)),this,SIGNAL(newConnection(QString)), Qt::DirectConnection);
        connect(channel_.get(),SIGNAL(clientConnected(QString)),this,SLOT(StartProcess(QString)), Qt::DirectConnection);

        connect(this,SIGNAL(SendToClient(unsigned char* ,unsigned int)),channel_.get(),SLOT(NewFrame(unsigned char* ,unsigned int)), Qt::DirectConnection);




        generatorThread->start();
        channelThread->start();





    }

    //====================================================================================
    void Stop(){

         stopped_.clear();
        emit StopSignal();
        emit stopped();

    }
    //====================================================================================
    void StartProcess(QString)
    {
        stopped_.test_and_set();
        while(true)
        {
             QCoreApplication::processEvents(QEventLoop::AllEvents);
if(!stopped_.test_and_set())
{
    return;
}
            //get desktop snapshot
             const auto copyOfFrame=FrameManager::GetInstance().GetCopyFromFrame();
             if(copyOfFrame.first!=nullptr && copyOfFrame.second!=0)
             {
                
             emit SendToClient(copyOfFrame.first,copyOfFrame.second);

             }

        }
    }
    //====================================================================================
    void Start() override
     {
        emit StartSignal(8084);
     }
     //====================================================================================


private:

 std::unique_ptr<FrameGenerator> frameGenerator_;
 std::unique_ptr<Channel> channel_;
 std::atomic_flag stopped_;
 QThread* generatorThread;
 QThread* channelThread;


};

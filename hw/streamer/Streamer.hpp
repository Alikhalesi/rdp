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
  void ShowOnScreen(unsigned char* data,unsigned int len);

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

        auto* generatorThread=new QThread();
        frameGenerator_->moveToThread(generatorThread);
        auto* channelThread=new QThread();
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

        emit StopSignal();
        emit stopped();
        stopped_.clear();
    }
    //====================================================================================
    void StartProcess(QString)
    {
        stopped_.test_and_set();
        while(stopped_.test_and_set())
        {
             QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

            //get desktop snapshot
             auto copyOfFrame=FrameManager::GetInstance().GetCopyFromFrame();
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

};

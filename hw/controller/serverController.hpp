#include <QObject>
#include <QDebug>
#include <streamer/Streamer.hpp>
#include <streamer/DesktopFrameGenerator.hpp>
#include <channel/p2pChannel.hpp>
#include <components/FrameImageProvider.hpp>

#include "managers/IDGXIFrameGenerator.hpp"
#pragma once
class ServerController: public QObject
{
    Q_OBJECT
public:
    ServerController();
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
 signals:
    void newConnection(QString cnnInfo);
    void imageChanged();
    void stopped();
    void startStream();
    void stopStream();
    public slots:
    void newFrame(unsigned char*,unsigned int);


private:
     //Streamer<DesktopFrameGenerator,P2PChannel> streamer{};
    Streamer<IDGXIFrameGenerator, P2PChannel> streamer{};
    
};

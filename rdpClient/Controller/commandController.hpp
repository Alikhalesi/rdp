#pragma once
#include <QObject>
#include "managers/localHookManager.hpp"
#include "model/command.hpp"
#include <QDebug>
#include <rxqt.hpp>
#include <ClientTcpChannel.h>
#include <QThread>
#include <stdlib.h>
#include <memory>
class CommandController:public QObject
{
    Q_OBJECT
public:
    CommandController(QObject* parent=nullptr);

   Q_INVOKABLE void start(const QString& ip);
   Q_INVOKABLE void stop();
    public slots:
    void newCommand(inputCommand);
    void onError(const QString& err);
private:
signals:
    void StartChannel(QString ip,int port);
    void SendCommand(std::shared_ptr<char[]> data,int len);
    void error(const QString& err);
    void disconnected();
    void connected();
private:
 void SendCommandToServer(inputCommand cmd);
 void StartCapture();
 void StopCapture();
 void SendClientResulotion();
 ClientTcpChannel* channel_;

QThread* channelThread_;

};

#pragma once
#include "IChannel.hpp"
#pragma once
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QtDebug>
#include <QCoreApplication>
#include <QEventLoop>
class P2PChannel:public QObject
{
Q_OBJECT
    // IChannel interface
public slots:
        void Start(int port);
        void Stop();
         void NewFrame(unsigned char* data,unsigned int len);
         void disconnectd();
          void NewClient();
signals:
        void clientConnected(QString cnnInfo);
        void clientDisconnected();
        void stopped();
private:
    void Listen(int port);

    void Send(unsigned char *data,unsigned int len);


private:
    QTcpServer* server_=nullptr;
    QTcpSocket* client_=nullptr;
};

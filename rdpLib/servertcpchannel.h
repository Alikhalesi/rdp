#ifndef SERVERTCPCHANNEL_H
#define SERVERTCPCHANNEL_H
#include <QObject>
#include <QTcpServer>
#include <QString>
#include <QTcpSocket>
#include "channel/IChannelInterceptor.hpp"
#include <vector>
#include <QVariant>
#include <model/command.hpp>
#include "rdpLib_global.h"
class RDPLIB_EXPORT ServerTcpChannel:public QObject
{
    Q_OBJECT
public:
    ServerTcpChannel(int port, QObject* parent= nullptr);
    ServerTcpChannel(std::vector<std::unique_ptr<IChannelInterceptor>>&& interceptors,int port, QObject* parent= nullptr);
public slots:
    void Start();
    void Stop();
   // void Send(QVariant data);

signals:
    void clientConnected(const QString& cnnInfo);
    void clientDisconnected();
    void dataReadyInternal(QVariant data);
    void dataReady(QVariant data);
private slots:
    void NewClient();
    void ClientDisconnectd();
    void readyRead();

private:
    int port_;
    QTcpServer* server_=nullptr;
    QTcpSocket* client_=nullptr;
std::vector<std::unique_ptr<IChannelInterceptor>> interceptors_;
};

#endif // SERVERTCPCHANNEL_H

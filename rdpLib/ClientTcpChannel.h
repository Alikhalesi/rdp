#ifndef CLIENTTCPCHANNEL_H
#define CLIENTTCPCHANNEL_H
#include "rdpLib_global.h"
#include <QObject>
#include <QTcpSocket>
#include "channel/IChannelInterceptor.hpp"
#include <vector>
#include <QVariant>
#include <Windows.h>
#include <QNetworkProxyFactory>
#include <model/command.hpp>
#include <mutex>
#include <stdlib.h>

class RDPLIB_EXPORT ClientTcpChannel : public QObject
{
    Q_OBJECT
public:
    explicit  ClientTcpChannel(QObject *parent = nullptr);
    explicit  ClientTcpChannel(std::vector<std::unique_ptr<IChannelInterceptor>>&& interceptors,QObject *parent = nullptr);

public :


 signals:
void connected();
void disconnected();
void dataReadyInternal(QVariant data);
void dataReady(QVariant data);

void Error(const QString& err);
public:
    void SetReadBufferSize(qint64 size);

QByteArray Receive();


 public slots:
void Disconnect();
void SocketError(QAbstractSocket::SocketError socketError);
void Send(const char* data,int len);
void SendAndDelete(std::shared_ptr<char[]> data,int len);
void Start(const QString& ip,int port);
private:
void ConnectToHost(const QString& ip,int port);
QTcpSocket* socket_=nullptr;
std::vector<std::unique_ptr<IChannelInterceptor>> interceptors_;
std::recursive_mutex syncLock_;
qint64 readBufferSize_ = 0;
 private slots:
void readyRead();


};

#endif // CLIENTTCPCHANNEL_H

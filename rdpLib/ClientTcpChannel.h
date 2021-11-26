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

public:

void Send(const char* data,int len);
QByteArray Receive();

void Disconnect();
 public slots:

void Start(const QString& ip,int port);
private:
void ConnectToHost(const QString& ip,int port);
QTcpSocket* socket_;
std::vector<std::unique_ptr<IChannelInterceptor>> interceptors_;

 private slots:
void readyRead();


};

#endif // CLIENTTCPCHANNEL_H

#pragma once
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
class P2PChannel:public QObject
{
    Q_OBJECT
public :
signals:
    void connected(QString cnnInfo);
    void disconnected();
public slots:
    void Connect();
    void Disconnect();
    void connectedSlot();
    void disconnectedSlot();
    void bytesWrittenSlot(qint64 bytes);
    void readyReadSlot();
private:
    QTcpSocket* client_;

};

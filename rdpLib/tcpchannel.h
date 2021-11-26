#ifndef TCPCHANNEL_H
#define TCPCHANNEL_H

#include <QObject>

class TcpChannel : public QObject
{
    Q_OBJECT
public:
    explicit TcpChannel(QObject *parent = nullptr);

signals:

};

#endif // TCPCHANNEL_H

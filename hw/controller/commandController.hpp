#pragma once
#include <QObject>
#include <servertcpchannel.h>
#include <channel/inputCommandInterceptor.hpp>
#include <model/command.hpp>
class CommandController:public QObject
{
    Q_OBJECT
public :
    explicit CommandController(QObject* parent=nullptr);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
public slots:
    void newCommand(QVariant);
private:
    ServerTcpChannel* channel_;

};

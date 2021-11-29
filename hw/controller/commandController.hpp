#pragma once
#include <QObject>
#include <servertcpchannel.h>
#include <channel/inputCommandInterceptor.hpp>
#include <model/command.hpp>
#include <managers/ICommandManager.hpp>
#include <memory>
#include <rxqt.hpp>
class CommandController:public QObject
{
    Q_OBJECT
public :
    explicit CommandController( std::unique_ptr<ICommandManager>&& commandManager,QObject* parent=nullptr);
    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
public slots:
    void newCommand(QVariant);

private:
    ServerTcpChannel* channel_;
    std::unique_ptr<ICommandManager> commandManager_;

};

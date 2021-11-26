#pragma once
#include <QObject>
#include "managers/localHookManager.hpp"
#include "model/command.hpp"
#include <QDebug>
#include <ClientTcpChannel.h>

class CommandController:public QObject
{
    Q_OBJECT
public:
    CommandController(QObject* parent=nullptr):QObject(parent){};

   Q_INVOKABLE void Start(const QString& ip,int port);
   Q_INVOKABLE void Stop();
    public slots:
    void newCommand(inputCommand);
private:
 void StartCapture();
 void StopCapture();
 ClientTcpChannel* channel_;



};

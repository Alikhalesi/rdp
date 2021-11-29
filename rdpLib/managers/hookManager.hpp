#pragma once
#include <Windows.h>
#include <string>
#include "model/command.hpp"
#include <QDebug>
#include "rdpLib_global.h"
#include <QObject>
#include <QThread>
#include <Windows.h>
#include <QThreadStorage>


class RDPLIB_EXPORT HookManager:public QObject
{
    Q_OBJECT
public:
    HookManager(QObject* parent=nullptr):QObject(parent){}

    void Start();
    void Stop();
void EmitNewCommand(inputCommand cmd);
signals:
    void newCommand(inputCommand cmd);


private:


HHOOK mouseHook_=nullptr;
HHOOK keyboardHook_=nullptr;
inputCommand lastCommand_;

};

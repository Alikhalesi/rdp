#pragma once
#include <Windows.h>
#include <string>
#include "model/command.hpp"
#include <QDebug>

#include <QObject>
#include <QThread>
class  LocalHookManager:public QObject
{
    Q_OBJECT
public:
    LocalHookManager(QObject* parent=nullptr):QObject(parent){}
    static LocalHookManager& GetInstance();
    void Start();
    void Stop();
void EmitNewCommand(inputCommand cmd);
signals:
    void newCommand(inputCommand cmd);


private:


HHOOK mouseHook_=nullptr;
HHOOK keyboardHook_=nullptr;

};

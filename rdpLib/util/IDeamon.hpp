#pragma once
#include <QObject>
#include <atomic>
#include <QCoreApplication>
#include <QEventLoop>
#include "rdpLib_global.h"
class RDPLIB_EXPORT IDeamon:public QObject
{
    Q_OBJECT
    protected:
    virtual void Run()=0;
public slots:
    void Start(QString);
     void Stop();


private:
     std::atomic_flag stopped_;
};

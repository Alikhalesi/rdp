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
   virtual void Start(QString);
     virtual void Stop();


protected:
     std::atomic_flag stopped_;
};

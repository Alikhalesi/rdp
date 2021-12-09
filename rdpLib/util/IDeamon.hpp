#pragma once
#include <QObject>
#include <atomic>
#include <QCoreApplication>
#include <QEventLoop>
class IDeamon:public QObject
{
    Q_OBJECT
    protected:
    virtual void Run()=0;
public slots:
    void Start();
     void Stop();


private:
     std::atomic_flag stopped_;
};

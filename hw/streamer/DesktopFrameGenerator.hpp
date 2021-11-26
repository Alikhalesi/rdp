#pragma once
#include <QObject>
#include <atomic>
#include <QtDebug>
#include <QEventLoop>
#include <QCoreApplication>
#include <managers/frameManager.hpp>
#include <util/imageUtil.hpp>

class DesktopFrameGenerator: public QObject
{
    Q_OBJECT
public:
    DesktopFrameGenerator();
public slots:
    void Start(QString s);
    void Stop();
    signals:
    void NewFrame(unsigned char* frameData,unsigned int len);
    void stopped();

protected:
        void UpdateCurrentFrame();
private :

    std::atomic_flag stopped_;

};

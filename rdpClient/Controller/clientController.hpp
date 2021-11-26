#pragma once
#include <QObject>
#include <ClientTcpChannel.h>
#include <components/FrameImageProvider.hpp>
#include <channel/LenValInterceptor.hpp>
#include <vector>
#include <memory>
#include <QVariant>
#include "commandController.hpp"

class ClientController:public QObject
{
Q_OBJECT
public:
    explicit ClientController(QObject* parent=nullptr);
    public: signals:
    void connected();
    void disconnected();
    void imageChanged();
public slots:
    void start(QString ipPort);
    void stop();
    void newImage(QVariant data);
private:
 ClientTcpChannel* channel_;


};

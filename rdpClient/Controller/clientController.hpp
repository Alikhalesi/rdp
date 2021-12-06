#pragma once
#include <QObject>
#include <ClientTcpChannel.h>
#include <components/FrameImageProvider.hpp>
#include <channel/LenValInterceptor.hpp>
#include <util/imageUtil.hpp>
#include <vector>
#include <memory>
#include <QVariant>
#include "commandController.hpp"
#include "managers/uiupdater.h"
class ClientController:public QObject
{
Q_OBJECT
public:
    explicit ClientController(QObject* parent=nullptr);
    public: signals:
    void connected();
    void disconnected();
    void imageChanged();
      void error(const QString& err);
      void startProcessing();
      void stopProcessing();
      void startChannelProcessing(QString ip,int port);
public slots:
    void start(QString ip);
    void stop();
    void newImage();
    void onError(const QString& err);
private:
 ClientTcpChannel* channel_;
 QThread* uiUpdaterThread_;
 QThread* channelThread_;
 ImageUtil imageUtil_;


};

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
      void error(const QString& err);
public slots:
    void start(QString ip);
    void stop();
    void newImage(QVariant data);
    void onError(const QString& err);
private:
 ClientTcpChannel* channel_;


};

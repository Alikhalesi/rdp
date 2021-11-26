#pragma once
#include <QObject>
#include <QVariant>
#include "rdpLib_global.h"
class RDPLIB_EXPORT IChannelInterceptor:public QObject
{
    Q_OBJECT
public:
    signals:
    void dataOutput(QVariant data);
public slots:
   virtual void dataInput(QVariant data)=0;
private:
    virtual void Accept(QVariant data)=0;

};

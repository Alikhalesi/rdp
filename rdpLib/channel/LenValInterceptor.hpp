#pragma once
#include "IChannelInterceptor.hpp"
#include <QVariant>
#include <QException>
#include <exception>
#include <stdexcept>
#include "rdpLib_global.h"
#include <QDebug>


class RDPLIB_EXPORT LenValInterceptor:public IChannelInterceptor
{
public:
    void dataInput(QVariant data) override;
private:
    void Accept(QVariant data) override;
    unsigned int dataSize_=0;
    QByteArray readedData_;


};

#pragma once
#include "IChannelInterceptor.hpp"
#include <QVariant>
#include <QException>
#include <exception>
#include <stdexcept>
#include "rdpLib_global.h"
#include <QDebug>
#include "model/command.hpp"


class RDPLIB_EXPORT InputCommandInterceptor:public IChannelInterceptor
{
public:
    void dataInput(QVariant data) override;
private:
    void Accept(QVariant data) override;



};

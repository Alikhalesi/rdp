#pragma once
#include <QObject>
#include <atomic>
#include <QCoreApplication>
#include <QEventLoop>
#include "util/IDeamon.hpp"
#include "rdpLib_global.h"
class RDPLIB_EXPORT IFrameGenerationManager :public IDeamon
{
protected:
	virtual void Run() = 0;

};

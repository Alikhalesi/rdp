#pragma once
#include <Windows.h>
#include "IFrameGenerationManager.hpp"
#include "util/IDGXI/CommonTypes.h"
#include "util/IDGXI/DisplayManager.h"
#include "util/IDGXI/DuplicationManager.h"
#include "util/imageUtil.hpp"
#include "rdpLib_global.h"
#include <thread>


#include "util/IDGXI/networkOutManager.hpp"
#include "util/IDGXI/OutputManager.h"
#include "util/IDGXI/ThreadManager.h"


class RDPLIB_EXPORT IDGXIFrameGenerator:public IFrameGenerationManager
{
public:
	IDGXIFrameGenerator();
	void Run() override;
	virtual void Start(QString) override;
	virtual void Stop() override;

	virtual ~IDGXIFrameGenerator();

private:
	void InitDX();
	int StartWork();
	HANDLE UnexpectedErrorEvent = nullptr;
	HANDLE ExpectedErrorEvent = nullptr;
	HANDLE TerminateThreadsEvent = nullptr;
	NetworkOutmanager OutMgr;
	THREADMANAGER ThreadMgr;
	
	
};



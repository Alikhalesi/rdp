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


class RDPLIB_EXPORT IDGXIFrameGenerator:public IFrameGenerationManager
{
public:
	IDGXIFrameGenerator();
	void Run() override;
	DUPLICATIONMANAGER duplicationMngr_;
	virtual ~IDGXIFrameGenerator();

private:
	int StartWork();
	HANDLE UnexpectedErrorEvent = nullptr;
	HANDLE ExpectedErrorEvent = nullptr;
	HANDLE TerminateThreadsEvent = nullptr;
	NetworkOutmanager OutMgr;
	
	
};



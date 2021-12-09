#pragma once

#include <Windows.h>
#include "IFrameGenerationManager.hpp"
#include "util/IDGXI/CommonTypes.h"
#include "util/IDGXI/DisplayManager.h"
#include "util/IDGXI/DuplicationManager.h"
class IDGXIFrameGenerator:public IFrameGenerationManager
{
public:
	void Run() override;
	DUPLICATIONMANAGER duplicationMngr_;
};



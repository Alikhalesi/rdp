#pragma once
#include "OutputManager.h"
class NetworkOutmanager:public OUTPUTMANAGER
{
public:
	DUPL_RETURN UpdateFrameBuffer(_In_ PTR_INFO* PointerInfo);
};
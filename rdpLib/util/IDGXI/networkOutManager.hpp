#pragma once
#include "OutputManager.h"
#include "managers/frameManager.hpp"
#include "util/imageUtil.hpp"
class NetworkOutmanager:public OUTPUTMANAGER
{
public:
	DUPL_RETURN UpdateFrameBuffer() const;
};
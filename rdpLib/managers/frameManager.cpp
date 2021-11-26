#include "managers/frameManager.hpp"
//=====================================================================================================
FrameManager::FrameManager()
{
    Init();
}
//=====================================================================================================
FrameManager& FrameManager::GetInstance()
{
    static FrameManager* frameManager=new FrameManager();
    return *frameManager;
}
//==================================================================================
FrameWriter FrameManager::GetBufferForWrite()
{
    return FrameWriter{*this};
}

//=====================================================================================================
std::pair<unsigned char*,unsigned int>  FrameManager::GetCopyFromFrame()
{
    std::lock_guard<SpinLock> lock {spinLock_};
if(currentFrame_[0]=='a' && currentFrame_[1]=='d')
{
    //already sent
    return std::make_pair(nullptr,0);
}

    memcpy(currentFrameCopy_,currentFrame_,frameLen_);
    currentFrame_[0]='a';
    currentFrame_[1]='d';
    return std::make_pair(currentFrameCopy_,frameLen_);
}
//=====================================================================================================
unsigned int FrameManager::GetFrameLen()
{
    return frameLen_;
}
//=====================================================================================================
void FrameManager::Init()
{
        currentFrame_=(unsigned char*) malloc(allocationLen_);
        currentFrameCopy_=(unsigned char*) malloc(allocationLen_);
}
//=====================================================================================================
FrameManager::~FrameManager()
{
    free(currentFrame_);
}



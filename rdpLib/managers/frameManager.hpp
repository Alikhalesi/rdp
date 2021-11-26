#pragma once
#include <cstdlib>
#include <mutex>
#include "util/spinLock.hpp"
#include <Windows.h>
#include "rdpLib_global.h"

class FrameWriter;
class RDPLIB_EXPORT FrameManager
{
public:
    friend FrameWriter;
    static FrameManager& GetInstance();
    FrameWriter GetBufferForWrite();

    std::pair<unsigned char*,unsigned int>  GetCopyFromFrame();

    unsigned int GetFrameLen();
    ~FrameManager();
private:
    FrameManager();
    void Init();
     static FrameManager instance_;
    unsigned char* currentFrame_=nullptr;
    unsigned char* currentFrameCopy_=nullptr;
    unsigned int frameLen_=0;
    unsigned int allocationLen_=1007452;
    SpinLock spinLock_;
};
class RDPLIB_EXPORT FrameWriter
{
public:
    FrameWriter(FrameManager& manager):manager_{manager}{
        manager_.spinLock_.lock();
    };
    unsigned char* operator *()
    {
        return manager_.currentFrame_;
    };
    void SetFrameLen(unsigned int frameLen)
    {
        manager_.frameLen_=frameLen;
    };
    ~FrameWriter()
    {
    manager_.spinLock_.unlock();
    };
private:
    FrameManager& manager_;

};

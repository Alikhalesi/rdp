#pragma once
#include "rdpLib_global.h"
#include "model/command.hpp"
class RDPLIB_EXPORT ICommandManager
{
public:
   virtual void ApplyCommand(inputCommand cmd)=0;
   virtual ~ICommandManager()=default;
};

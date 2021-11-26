#pragma once
#include "ICommandManager.hpp"
class RDPLIB_EXPORT DefaultCommandManager: public ICommandManager
{


    // ICommandManager interface
public:
    void ApplyCommand(inputCommand cmd);
private :
    void ApplyCommand(MouseCommand cmd);
    void ApplyCommand(KeyboardCommand cmd);
};


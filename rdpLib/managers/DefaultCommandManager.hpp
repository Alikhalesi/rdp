#pragma once
#include "ICommandManager.hpp"
#include <Windows.h>
#include <QDebug>
class RDPLIB_EXPORT DefaultCommandManager: public ICommandManager
{

public:
    DefaultCommandManager();
    // ICommandManager interface
public:
    void ApplyCommand(inputCommand cmd);
private :
    void ApplyCommand(MouseCommand cmd);
    void ApplyCommand(KeyboardCommand cmd);
    void ApplyCommand(InfoCommand cmd);
    ScreenResolution clientResolution_;
    ScreenResolution SystemResolution_;
};


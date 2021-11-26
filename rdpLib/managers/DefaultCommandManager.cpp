#include "DefaultCommandManager.hpp"
void DefaultCommandManager::ApplyCommand(inputCommand cmd)
{
    const auto type=cmd.type;
    switch (type) {
                    case commandType::Mouse:
                    {
                        ApplyCommand(cmd.cmd.mouseCommand);
                        break;
                    }
                    case commandType::Keyboard:
                    {
                        ApplyCommand(cmd.cmd.keyboardCommand);
                        break;
                    }



                  }
    return;

}
//==========================================================================================
void DefaultCommandManager::ApplyCommand(MouseCommand cmd)
{

}
//==========================================================================================
void DefaultCommandManager::ApplyCommand(KeyboardCommand cmd)
{

}
//==========================================================================================

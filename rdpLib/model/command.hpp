#pragma once
#include <QObject>
#include <QTime>
struct ScreenResolution
{
    int height;
    int width;
};

//
enum class commandType
{
    Keyboard,Mouse,Info
};
struct MouseCommand
{
    int code;//move,leftbtnclick,..
    int x;
    int y;
};
struct KeyboardCommand
{
    int code;//keypress,keydown
    int keyCode;
};
struct InfoCommand
{
    ScreenResolution res;
};
struct inputCommand
{
    commandType type;
   // QTime occurance;
    union command
    {
         MouseCommand mouseCommand;
         KeyboardCommand keyboardCommand;
         InfoCommand infoCommand;

    } cmd;

};
Q_DECLARE_METATYPE(inputCommand);


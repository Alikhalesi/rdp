#pragma once
#include <QObject>
enum class commandType
{
    Keyboard,Mouse
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

struct inputCommand
{
    commandType type;
    union command
    {
         MouseCommand mouseCommand;
         KeyboardCommand keyboardCommand;

    } cmd;

};
Q_DECLARE_METATYPE(inputCommand);


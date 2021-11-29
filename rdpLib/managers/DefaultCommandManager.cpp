#include "DefaultCommandManager.hpp"
DefaultCommandManager::DefaultCommandManager()
{

    SystemResolution_.width=GetSystemMetrics(SM_CXSCREEN);
    SystemResolution_.height=GetSystemMetrics(SM_CYSCREEN);
    qDebug()<<"scr res:"<< SystemResolution_.width<<","<<SystemResolution_.height;
}
//==================================================================
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
    case commandType::Info:
    {
        ApplyCommand(cmd.cmd.infoCommand);
        break;
    }



                  }
    return;

}
//==========================================================================================
void DefaultCommandManager::ApplyCommand(MouseCommand cmd)
{
       qDebug()<<"client res:"<<clientResolution_.width <<","<<clientResolution_.height ;
       qDebug()<<"cmd x,y:"<<cmd.x <<","<<cmd.y;

    int x=cmd.x / ((double) clientResolution_.width /(double)  SystemResolution_.width);
    int y=cmd.y / ((double)clientResolution_.height / (double) SystemResolution_.height);
    qDebug()<<" x,y:"<<x <<","<<y;

    INPUT input{0};
    input.type=INPUT_MOUSE;
    input.mi.dx=x;
    input.mi.dy=y;
    input.mi.mouseData=0;

    switch(cmd.code)
    {
    //mouse move
    case 0x0200:
    {
       // SetCursorPos(x,y);
       input.mi.dwFlags= MOUSEEVENTF_MOVE;
        break;
    }
    //lbuttondown
    case 0x0201:
    {
        input.mi.dwFlags=MOUSEEVENTF_LEFTDOWN;

        break;
    }
        //lbutton up
    case  0x0202:
    {
         input.mi.dwFlags=MOUSEEVENTF_LEFTUP;
        break;
    }
        //dblClick
    case  0x0203:
    {
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;

            SendInput(1,&input,sizeof(INPUT));
            SendInput(1,&input,sizeof(INPUT));



            input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;

            SendInput(1,&input,sizeof(INPUT));
        return;
    }
        //rbutton down
    case 0x0204:
    {
        input.mi.dwFlags=MOUSEEVENTF_RIGHTDOWN;
       break;
    }
        //rbutton up
    case 0x0205:
    {
        input.mi.dwFlags=MOUSEEVENTF_RIGHTUP;
       break;
    }
    }
  SendInput(1,&input,sizeof(INPUT));

}
//==========================================================================================
void DefaultCommandManager::ApplyCommand(KeyboardCommand cmd)
{
 INPUT input{0};
 input.type=INPUT_KEYBOARD;

}
//==========================================================================================
void DefaultCommandManager::ApplyCommand(InfoCommand cmd)
{
 clientResolution_=cmd.res;
}
//==========================================================================================

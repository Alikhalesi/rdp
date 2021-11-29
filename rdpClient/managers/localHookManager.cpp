#include "managers/localHookManager.hpp"

//==================================================================
LRESULT CALLBACK MouseProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{

    qDebug()<<"Hook MouseProc Called"<<wParam;
MOUSEHOOKSTRUCT* info=(MOUSEHOOKSTRUCT*) lParam;

inputCommand cmd;
//cmd.occurance=QTime::currentTime();
cmd.type=commandType::Mouse;
cmd.cmd.mouseCommand.code=wParam;
cmd.cmd.mouseCommand.x=info->pt.x;
cmd.cmd.mouseCommand.y=info->pt.y;

LocalHookManager::GetInstance().EmitNewCommand(cmd);
    return    CallNextHookEx(0,nCode,wParam,lParam);

}
//==================================================================
LRESULT CALLBACK KeyboardProc(
  _In_ int    code,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
    inputCommand cmd;
    cmd.type=commandType::Keyboard;
    //cmd.occurance=QTime::currentTime();
    cmd.cmd.keyboardCommand.keyCode=wParam;
    cmd.cmd.keyboardCommand.code=lParam;
 qDebug()<<"Hook KeyboardProc Called";

 LocalHookManager::GetInstance().EmitNewCommand(cmd);
   return    CallNextHookEx(0,code,wParam,lParam);
}
//==================================================================
LocalHookManager& LocalHookManager::GetInstance()
{
    static LocalHookManager * instance_=new LocalHookManager();
    return *instance_;
}
//==================================================================
void LocalHookManager::Start()
{
     mouseHook_= SetWindowsHookExA(WH_MOUSE,MouseProc,0,GetThreadId(GetCurrentThread()));
     if(mouseHook_==nullptr)
     {
         MessageBoxA(0,(std::to_string(GetLastError()) +"mouseHook_").c_str(),0,0);
     }
     keyboardHook_= SetWindowsHookExA(WH_KEYBOARD,KeyboardProc,0,GetThreadId(GetCurrentThread()));
     if(keyboardHook_==nullptr)
     {
         MessageBoxA(0,(std::to_string(GetLastError()) +"keyboardHook_").c_str(),0,0);
     }
}
//==================================================================
void LocalHookManager::Stop()
{
    if(mouseHook_!=nullptr)
    {
        UnhookWindowsHookEx(mouseHook_);
    }
    if(keyboardHook_!=nullptr)
    {
        UnhookWindowsHookEx(keyboardHook_);
    }
}
//==================================================================
void LocalHookManager::EmitNewCommand(inputCommand cmd)
{
    emit newCommand(cmd);
}
//==================================================================

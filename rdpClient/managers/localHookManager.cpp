#include "managers/localHookManager.hpp"

//==================================================================
LRESULT CALLBACK MouseProc(
  _In_ int    nCode,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
)
{
    MouseCommand mouseCommand{0};
    qDebug()<<"Hook MouseProc Called"<<wParam;
MOUSEHOOKSTRUCT* info=(MOUSEHOOKSTRUCT*) lParam;
mouseCommand.x=info->pt.x;
mouseCommand.y=info->pt.y;
mouseCommand.code=wParam;
inputCommand cmd;
cmd.type=commandType::Mouse;
cmd.cmd.mouseCommand=mouseCommand;

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

 qDebug()<<"Hook KeyboardProc Called";
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

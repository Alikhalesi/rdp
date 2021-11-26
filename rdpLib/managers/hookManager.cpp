#include "managers/hookManager.hpp"
static HMODULE GetThisDllHandle()
{
  MEMORY_BASIC_INFORMATION info;
  size_t len = VirtualQueryEx(GetCurrentProcess(), (void*)GetThisDllHandle, &info, sizeof(info));
  assert(len == sizeof(info));
  return len ? (HMODULE)info.AllocationBase : NULL;
}
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

HookManager::GetInstance().EmitNewCommand(cmd);
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
HookManager& HookManager::GetInstance()
{
    static HookManager * instance_=new HookManager();
    return *instance_;
}
//==================================================================
void HookManager::Start()
{
     mouseHook_= SetWindowsHookExA(WH_MOUSE,MouseProc,GetThisDllHandle(),GetThreadId(QThread::currentThreadId()));
     if(mouseHook_==nullptr)
     {
         MessageBoxA(0,(std::to_string(GetLastError()) +"mouseHook_").c_str(),0,0);
     }
     keyboardHook_= SetWindowsHookExA(WH_KEYBOARD,KeyboardProc,GetThisDllHandle(),GetThreadId(QThread::currentThreadId()));
     if(keyboardHook_==nullptr)
     {
         MessageBoxA(0,(std::to_string(GetLastError()) +"keyboardHook_").c_str(),0,0);
     }
}
//==================================================================
void HookManager::Stop()
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
void HookManager::EmitNewCommand(inputCommand cmd)
{
    emit newCommand(cmd);
}
//==================================================================

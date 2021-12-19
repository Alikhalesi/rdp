#include "uiupdater.h"

uiUpdater::uiUpdater(QObject *parent) : QObject(parent)
{
     showWindow =std::make_unique<QWindow>();
     const auto hwnd = (HWND)showWindow->winId();
     LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
     lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
     SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);

     SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

   //  ShowWindow(hWnd, SW_SHOW); //display window
  
     showWindow->showMaximized();
  //  showWindow->show();
}
//==============================================================================
uiUpdater *uiUpdater::GetInstance()
{
    static auto* instance=new uiUpdater();
    return instance;
}
//==============================================================================
void uiUpdater::start()
{
    stopped_.test_and_set();
    while(stopped_.test_and_set())
    {
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
//Sleep(1000);
        if(!stopped_.test_and_set())
        {
            return;
        }

        UpdateImage();
    }
}
//==============================================================================
void uiUpdater::stop()
{
    stopped_.clear();
}
//==============================================================================
void uiUpdater::UpdateImage()
{
    auto copyOfFrame=FrameManager::GetInstance().GetCopyFromFrame();
    if(copyOfFrame.first!=nullptr && copyOfFrame.second!=0)
    {
     // FrameImageProvider::GetInstance()->SetImageData(copyOfFrame);
       // emit newImage();

        imageUtil_.ConvertJPegToHbitmap(copyOfFrame.first,copyOfFrame.second,(HWND)showWindow->winId());
    }

}
//==============================================================================

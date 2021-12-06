#include "uiupdater.h"

uiUpdater::uiUpdater(QObject *parent) : QObject(parent)
{
     showWindow =std::make_unique<QWindow>();
    showWindow->show();
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

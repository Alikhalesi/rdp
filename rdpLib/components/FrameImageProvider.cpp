#include "FrameImageProvider.hpp"
FrameImageProvider* FrameImageProvider::instance_=nullptr;
//========================================================================================================
FrameImageProvider::FrameImageProvider(ImageType type, Flags flags)
    : QQuickImageProvider(type, flags)
{

}
//========================================================================================================
QPixmap FrameImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
     qDebug()<<"FrameImageProvider::requestPixmap";
    static int demo=1;
    int width = 10000;
       int height = 10000;

       if (size)
       {
           *size = QSize(width, height);
       }

       QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
                      requestedSize.height() > 0 ? requestedSize.height() :
                                                   height);
       pixmap.fill(QColor("red").rgba());
       QPainter painter(&pixmap);
       QFont f = painter.font();
       f.setPixelSize(20);
       f.setBold(true);
       painter.setFont(f);
       painter.setPen(Qt::blue);
       if (requestedSize.isValid())
           painter.scale(requestedSize.width() / width,
                         requestedSize.height() / height);
       painter.drawText(QRectF(0, 0, width, height), Qt::AlignCenter, QString::number(1212121212));

       return pixmap;
}
//========================================================================================================
QImage FrameImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    std::lock_guard lock(frameLock_);
    qDebug()<<"FrameImageProvider::requestImage";
    return frame_;
}
//========================================================================================================
FrameImageProvider* FrameImageProvider::GetInstance()
{
        if(instance_==nullptr)
        {
            instance_=new FrameImageProvider(QQuickImageProvider::Image,QQuickImageProvider::ForceAsynchronousImageLoading);
        }
        return instance_;
}
//========================================================================================================
void FrameImageProvider::SetImageData(std::pair<unsigned char *, unsigned int> data)
{
std::lock_guard lock(frameLock_);
    frame_.loadFromData(data.first,data.second,"JPG");
}
//========================================================================================================

#pragma once
#include <QQuickImageProvider>
#include <QPainter>
#include <QPixmap>
#include <QFont>
#include <QObject>
#include <QDebug>
#include <mutex>
#include "rdpLib_global.h"
class RDPLIB_EXPORT FrameImageProvider: public QQuickImageProvider
{
public:



   QPixmap requestPixmap(const QString& id,
                                        QSize* size,
                                        const QSize& requestedSize);

   QImage requestImage(const QString& id,
                                      QSize* size,
                                      const QSize& requestedSize);
   static FrameImageProvider* GetInstance();
   void SetImageData( std::pair<unsigned char*,unsigned int> data);

private :
   FrameImageProvider(ImageType type, Flags flags);
   std::pair<unsigned char*,unsigned int> imageLen_;
   static FrameImageProvider* instance_;
   QImage frame_;
std::mutex frameLock_;

};

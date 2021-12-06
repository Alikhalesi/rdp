#ifndef UIUPDATER_H
#define UIUPDATER_H

#include <QObject>
#include <atomic>
#include <QCoreApplication>
#include <QEventLoop>
#include <managers/frameManager.hpp>
#include <components/FrameImageProvider.hpp>
#include <util/imageUtil.hpp>
#include <QWindow>
#include <memory>
class uiUpdater : public QObject
{
    Q_OBJECT
public:
        static uiUpdater* GetInstance();

public slots:
    void start();
    void stop();

    signals:
    void newImage();

private:
        explicit uiUpdater(QObject *parent = nullptr);
    std::atomic_flag stopped_;
    void UpdateImage();
    static uiUpdater* instance_;
    ImageUtil imageUtil_;
     std::unique_ptr<QWindow> showWindow;
};

#endif // UIUPDATER_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <rxqt.hpp>
#include <QLocale>
#include <QTranslator>
#include "streamer/Streamer.hpp"
#include "streamer/DesktopFrameGenerator.hpp"
#include "channel/p2pChannel.hpp"
#include <QDebug>
#include "controller/serverController.hpp"
#include <QQmlContext>
#include "components/FrameImageProvider.hpp"
#include "controller/commandController.hpp"
#include "managers/DefaultCommandManager.hpp"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
  //  SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    QGuiApplication app(argc, argv);
  //rxqt::run_loop rxqt_run_loop;
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "hw_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator); 
            break;
        }
    }


    //





    //

    QQmlApplicationEngine engine;
auto* serverController=new ServerController();
auto* commandController=new CommandController(std::make_unique<DefaultCommandManager>());
    engine.rootContext()->setContextProperty( "_serverController", serverController );
engine.rootContext()->setContextProperty( "_commandController", commandController );
//serverController->start();
//engine.addImageProvider("_frameImageProvider",FrameImageProvider::GetInstance() );


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);

    }, Qt::QueuedConnection);

















    engine.load(url);

    return app.exec();
}

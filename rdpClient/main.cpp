#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <rxqt.hpp>
#include "Controller/clientController.hpp"
#include "Controller/commandController.hpp"
#include <components/FrameImageProvider.hpp>
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
rxqt::run_loop rxqt_run_loop;
    auto* clientController=new ClientController();
     auto* commandController=new CommandController();
     engine.rootContext()->setContextProperty( "_clientController", clientController );
  engine.rootContext()->setContextProperty( "_commandController", commandController );

    engine.addImageProvider("_frameImageProvider",FrameImageProvider::GetInstance() );



    const QUrl url(QStringLiteral("qrc:/ui/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "Controller/clientController.hpp"
#include "Controller/commandController.hpp"
#include <components/FrameImageProvider.hpp>
#include <components/FrameWriter.hpp>
#include <QWindow>
#include <QQuickView>
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    auto* clientController=new ClientController();
     auto* commandController=new CommandController();
     engine.rootContext()->setContextProperty( "_clientController", clientController );
  engine.rootContext()->setContextProperty( "_commandController", commandController );

   // engine.addImageProvider("_frameImageProvider",FrameImageProvider::GetInstance() );
 // qmlRegisterType<ImageWriter>("ImageWriter",1,0,"ImageWriter");



    const QUrl url(QStringLiteral("qrc:/ui/toolbar.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

 /*   QQuickView* view = new QQuickView;
    const auto engine = view->engine();
        auto* clientController=new ClientController();
     auto* commandController=new CommandController();
     engine->rootContext()->setContextProperty( "_clientController", clientController );
  engine->rootContext()->setContextProperty( "_commandController", commandController );
  const QUrl url(QStringLiteral("qrc:/ui/main.qml"));
    view->setSource(url);
    view->show();
    view->h*/
	
    return app.exec();
}

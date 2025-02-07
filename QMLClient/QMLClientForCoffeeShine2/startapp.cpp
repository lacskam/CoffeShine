#include "startapp.h"

StartApp::StartApp(QObject *parent) : QObject(parent)
{

    engine = new QQmlApplicationEngine(this); // Создаём движок qml

    interlayerForQML = new InterlayerForQML(this);    // Создаём класс соедияняющий qml и сpp

    connect(engine,SIGNAL(warnings(QList<QQmlError>)),this,SLOT(saveQmlEngineWarnings(QList<QQmlError>)));

    engine->rootContext()->setContextProperty("interlayerForQML", interlayerForQML);
    engine->rootContext()->setContextProperty("qmlProperty", interlayerForQML->qmlProperty);
    engine->addImageProvider(QLatin1String("drinks"), interlayerForQML->drinkImageProvider);

    engine->load(QUrl(QStringLiteral("qrc:/QMLFile/mainView.qml")));

    if (engine->rootObjects().isEmpty())
        qApp->exit(-1);
}

StartApp::~StartApp()
{
    interlayerForQML->deleteLater();
    engine->deleteLater();
}

void StartApp::saveQmlEngineWarnings(QList<QQmlError> listError)
{
   QString folder=QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
   QFile file(folder + "/qmlEngineError.txt");

   for(int i=0;i<listError.count();i++)
   {
       if (file.open(QIODevice::Append)) {
           QTextStream stream( &file );
           stream << "\n" << QDateTime::currentDateTime().toString()<< ": " << listError.at(i).toString() << Qt::endl;
       }
   }

   file.close();
}

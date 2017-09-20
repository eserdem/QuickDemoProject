#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "InformationBinder.h"
#include "InformationFetcher.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    InformationFetcher fetcher;
    InformationBinder binder(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    fetcher.fetchCpuInfo();
    const int coreCount = fetcher.getCoreCount();

    for(int currCore = 0; currCore < coreCount; currCore++)
    {
        binder.visualizeCoreInformation(currCore, *fetcher.getCoreInfo(currCore));
    }

    return app.exec();
}

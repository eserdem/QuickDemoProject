#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "InformationBinder.h"
#include "InformationFetcher.h"
#include "StatsFetcher.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    InformationBinder binder(engine);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    InformationFetcher cpuinfoSource;
    cpuinfoSource.fetchCpuInfo();

    const int coreCount = cpuinfoSource.getCoreCount();
    for(int currCore = 0; currCore < coreCount; currCore++)
    {
        binder.visualizeCoreInformation(currCore, *cpuinfoSource.getCoreInfo(currCore));
    }

    StatsFetcher statsSource;
    statsSource.startFetchingInfo();

    binder.visualizeStats(*statsSource.getCoreStats());

    return app.exec();
}

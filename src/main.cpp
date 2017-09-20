#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "InformationBinder.h"
#include "InformationFetcher.h"
#include "StatsFetcher.h"

int main(int argc, char *argv[])
{
    // Create basics, also prepare binder object ready to provide any possible model references to come from loading
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    InformationBinder binder(engine);

    // Load main QML model
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // Prepare cpu info fetcher, and get data ready
    InformationFetcher cpuinfoSource;
    cpuinfoSource.fetchCpuInfo();

    // Depending on core count, access data models and associate the models with the corresponding contexes
    const int coreCount = cpuinfoSource.getCoreCount();
    for(int currCore = 0; currCore < coreCount; currCore++)
    {
        if(QStandardItemModel* pItemModel = cpuinfoSource.getCoreInfo(currCore))
        {
            binder.visualizeCoreInformation(currCore, *pItemModel);
        }
    }

    // Prepare cpu status fetcher, set it up and get it to cyclic running state
    StatsFetcher statsSource;
    statsSource.startFetchingInfo();

    // Bind model of cyclic-updated status data to view
    if(QStandardItemModel* pItemModel = statsSource.getCoreStats())
    {
        binder.visualizeStats(*pItemModel);
    }

    return app.exec();
}

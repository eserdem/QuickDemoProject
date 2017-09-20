#include "InformationBinder.h"

#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQmlContext>
#include <QStandardItemModel>

InformationBinder::InformationBinder(QQmlApplicationEngine& engine)
    : m_engine(engine)
{
    // Prepare dummy binding to avoid missing model case for initial load
    static QStringList dummyDataList;
    dummyDataList.append("Fetching ...");

    // Set dummy model as context property
    QQmlContext* pContext = engine.rootContext();
    if(pContext)
    {
        pContext->setContextProperty("myCustomModel", QVariant::fromValue(dummyDataList));
    }
}

void InformationBinder::visualizeTabCommon(const QString& tabName, const QUrl& qmlTemplate, QStandardItemModel& coreDataModel)
{
    // Find the special view where tabs can be hooked under
    QObject* pFirstRootObject = m_engine.rootObjects().first();
    if(!pFirstRootObject)
    {
        // Initial QML loading probably went wrong
        return;
    }

    QObject* pTabViewObject = pFirstRootObject->findChild<QObject*>("tabView");
    if(!pTabViewObject)
    {
        // Binding couldn't found from UI side
        return;
    }

    // Load Tab Template
    QQmlComponent tabComponent(&m_engine, qmlTemplate);
    QVariant returnedValue;

    // Insert the tab loaded from template
    QMetaObject::invokeMethod(pTabViewObject, "addTab",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, tabName), Q_ARG(QVariant, QVariant::fromValue(&tabComponent)));

    // From instantiated tab, get returned tab object
    QObject* pTabObject = qvariant_cast<QObject*>(returnedValue);

    if(!pTabObject)
    {
        // Loading expected design from template Qml failed
        return;
    }

    // Force load Tab contents by setting active property to true
    pTabObject->setProperty("active", true);

    // Find TableView object located inside tab
    QObject* pTableObject = pTabObject->findChild<QObject*>("CoreContentsTableView");

    if(!pTableObject)
    {
        // Loading expected design from template Qml failed
        return;
    }

    // Assigned provided model
    pTableObject->setProperty("model", QVariant::fromValue(&coreDataModel));
}

void InformationBinder::visualizeCoreInformation(unsigned int coreId, QStandardItemModel& coreDataModel)
{
    // Prepare Tab name and pass specified QML template for initialization and binding
    const QString title = "Core " + QString::number(coreId);
    visualizeTabCommon(title, QUrl(QStringLiteral("qrc:/CoreTemplate.qml")), coreDataModel);
}

void InformationBinder::visualizeStats(QStandardItemModel& statsDataModel)
{
    // Prepare Tab name and pass specified QML template for initialization and binding
    const QString title = "CPU Utilization";
    visualizeTabCommon(title, QUrl(QStringLiteral("qrc:/StatsTemplate.qml")), statsDataModel);
}

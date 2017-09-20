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

    QQmlContext* pContext = engine.rootContext();
    pContext->setContextProperty("myCustomModel", QVariant::fromValue(dummyDataList));
}

void InformationBinder::visualizeCoreInformation(unsigned int coreId, QStandardItemModel& coreDataModel)
{
    // Find the special view where tabs can be hooked under
    QObject* pTabViewObject = m_engine.rootObjects().first()->findChild<QObject*>("tabView");

    if(!pTabViewObject)
    {
        // Binding couldn't found from UI side
        return;
    }

    // Load Tab Template
    QQmlComponent tabComponent(&m_engine, QUrl(QStringLiteral("qrc:/TabTemplate.qml")));

    QVariant returnedValue;
    QVariant title = "Core " + QString::number(coreId);

    // Insert the tab loaded from template
    QMetaObject::invokeMethod(pTabViewObject, "addTab",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, title), Q_ARG(QVariant, QVariant::fromValue(&tabComponent)));

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

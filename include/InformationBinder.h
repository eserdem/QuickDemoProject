#ifndef INFORMATIONBINDER_H
#define INFORMATIONBINDER_H

class QQmlApplicationEngine;
class QStandardItemModel;
class QUrl;
class QString;

// This class helps binding data models to QML views
class InformationBinder
{
public:
    InformationBinder(QQmlApplicationEngine& engine);

    // Visualizes CPU-Core information as a new Tab on UI
    void visualizeCoreInformation(unsigned int coreId, QStandardItemModel& coreDataModel);

    // Visualizes CPU load-status informaiton as an extra Tab on UI
    void visualizeStats(QStandardItemModel& statsDataModel);

private:
    // Internally used to yield common Tab allocation and Data binding tasks
    void visualizeTabCommon(const QString& tabName, const QUrl& qmlTemplate, QStandardItemModel& coreDataModel);

private:
    QQmlApplicationEngine& m_engine;

};

#endif // INFORMATIONBINDER_H

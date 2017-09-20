#ifndef INFORMATIONBINDER_H
#define INFORMATIONBINDER_H

class QQmlApplicationEngine;
class QStandardItemModel;
class QUrl;
class QString;

class InformationBinder
{
public:
    InformationBinder(QQmlApplicationEngine& engine);

    void visualizeCoreInformation(unsigned int coreId, QStandardItemModel& coreDataModel);
    void visualizeStats(QStandardItemModel& statsDataModel);

private:
    void visualizeTabCommon(const QString& tabName, const QUrl& qmlTemplate, QStandardItemModel& coreDataModel);

private:
    QQmlApplicationEngine& m_engine;

};

#endif // INFORMATIONBINDER_H

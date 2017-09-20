#ifndef INFORMATIONBINDER_H
#define INFORMATIONBINDER_H

class QQmlApplicationEngine;
class QStandardItemModel;

class InformationBinder
{
public:
    InformationBinder(QQmlApplicationEngine& engine);

    void visualizeCoreInformation(unsigned int coreId, QStandardItemModel& coreDataModel);

private:
    QQmlApplicationEngine& m_engine;

};

#endif // INFORMATIONBINDER_H

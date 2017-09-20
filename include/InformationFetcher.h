#ifndef INFORMATIONFETCHER_H
#define INFORMATIONFETCHER_H

#include <QList>
#include <QPair>
#include <QString>

class QStandardItemModel;
class QStringList;

class InformationFetcher
{
public:
    InformationFetcher();
    ~InformationFetcher();

    // Fetches cpu information prepares information data available. Data can be queried after this point
    void fetchCpuInfo();

    // Returns number of availble CPU cores
    int getCoreCount() const;

    // For given CPU core, returns data model containing data table
    QStandardItemModel* getCoreInfo(int coreIndex) const;

private:
    typedef QPair<QString, QString> CoreInfoItem;
    typedef QList<CoreInfoItem>     CoreInfoList;
    typedef QList<CoreInfoList>     CoreDatabase;

    void buildDatabaseFromCoreInfo(const QStringList& lines, CoreDatabase& coreDb);
    void constructDataModelFromDatabase(const CoreDatabase& coreDb);

private:
    QList<QStandardItemModel*>  m_coreInfos;



};

#endif // INFORMATIONFETCHER_H

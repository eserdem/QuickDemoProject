#include "InformationFetcher.h"

#include <QProcess>
#include <QStringList>
#include <QRegularExpression>
#include <QStandardItemModel>

InformationFetcher::InformationFetcher()
{
}

InformationFetcher::~InformationFetcher()
{
}

void InformationFetcher::constructDataModelFromDatabase(const CoreDatabase& coreDb)
{
    int currentModelIndex = 0;
    int currentRow = 0;
    for(CoreDatabase::const_iterator listItr = coreDb.cbegin(); listItr != coreDb.cend(); ++listItr)
    {
        if(m_coreInfos.size() <= currentModelIndex || NULL == m_coreInfos[currentModelIndex])
        {
            m_coreInfos.push_back(new QStandardItemModel((*listItr).size(), 2));
        }

        QStandardItemModel* pCurrentModel = m_coreInfos[currentModelIndex];
        pCurrentModel->clear();
        currentRow = 0;

        QMap<int, QVariant> mapRoles;

        for(CoreInfoList::const_iterator itemItr = (*listItr).cbegin(); itemItr != (*listItr).cend(); ++itemItr)
        {
            mapRoles.insert( Qt::DisplayRole, itemItr->first);
            mapRoles.insert( Qt::DecorationRole, itemItr->second);

            QStandardItem* pRowItem = new QStandardItem();

            pCurrentModel->setItem(currentRow, pRowItem);
            pCurrentModel->setItemData(pRowItem->index(), mapRoles);

            currentRow++;
        }

        currentModelIndex++;
    }

    // Empty remaining
    for(int i = currentModelIndex; i < m_coreInfos.size(); i++)
    {
        m_coreInfos[currentModelIndex]->clear();
    }

}

void InformationFetcher::buildDatabaseFromCoreInfo(const QStringList& lines, CoreDatabase& coreDb)
{
    const int lineCount = lines.size();
    CoreInfoList* pCurrentList = 0;

    for(int i = 0; i<lineCount; ++i)
    {
        if(!lines[i].contains(":"))
        {
            continue;
        }

        const QStringList words = lines[i].split(":", QString::SkipEmptyParts);
        if(words.size() < 2)
        {
            continue;
        }

        if(words[0].trimmed() == "processor")
        {
            coreDb.push_back(CoreInfoList());
            pCurrentList = &(coreDb[coreDb.size()-1]);
        }
        else
        {
            if(pCurrentList)
            {
                pCurrentList->push_back(CoreInfoItem(words[0].trimmed(), words[1].trimmed()));
            }
        }
    }
}

void InformationFetcher::fetchCpuInfo()
{
    QProcess fetchProcess;
    fetchProcess.start("sh", QStringList() << "-c" << "cat /proc/cpuinfo");
    fetchProcess.waitForFinished(-1);

    QString allResult = fetchProcess.readAllStandardOutput();
    QStringList lines = allResult.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    CoreDatabase db;
    buildDatabaseFromCoreInfo(lines, db);
    constructDataModelFromDatabase(db);
}

int InformationFetcher::getCoreCount() const
{
    return m_coreInfos.size();
}

QStandardItemModel* InformationFetcher::getCoreInfo(int coreIndex) const
{
    if(coreIndex < m_coreInfos.size())
    {
        return m_coreInfos[coreIndex];
    }

    return NULL;
}

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

    // Iterate through all list entries found in database
    for(CoreDatabase::const_iterator listItr = coreDb.cbegin(); listItr != coreDb.cend(); ++listItr)
    {
        // If we do yet lack a model for this cpu-core, we can create it now on-the-fly and register.
        if(m_coreInfos.size() <= currentModelIndex || NULL == m_coreInfos[currentModelIndex])
        {
            m_coreInfos.push_back(new QStandardItemModel((*listItr).size(), 2));
        }

        // Get model object ready and clear any previous entries (for case of updated parameters etc.)
        QStandardItemModel* pCurrentModel = m_coreInfos[currentModelIndex];
        pCurrentModel->clear();
        currentRow = 0;

        QMap<int, QVariant> mapRoles;

        // Loop over existing key-value couples and set the itemData using RoleBased fetch via the TableView
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

    // Empty remaining model items, if this could ever be the case
    for(int i = currentModelIndex; i < m_coreInfos.size(); i++)
    {
        m_coreInfos[currentModelIndex]->clear();
    }

}

void InformationFetcher::buildDatabaseFromCoreInfo(const QStringList& lines, CoreDatabase& coreDb)
{
    // Find out how many lines will need processing
    const int lineCount = lines.size();
    CoreInfoList* pCurrentList = 0;

    // Iterate through all lines
    for(int i = 0; i<lineCount; ++i)
    {
        // We are only interested in the lines where a key:value couple is provided
        if(!lines[i].contains(":"))
        {
            continue;
        }

        // If there is an unexpected syntax, we skip it smoothly
        const QStringList words = lines[i].split(":", QString::SkipEmptyParts);
        if(words.size() < 2)
        {
            continue;
        }

        // Following string below determines information that we are about to start new core information flow
        if(words[0].trimmed() == "processor")
        {
            coreDb.push_back(CoreInfoList());
            pCurrentList = &(coreDb[coreDb.size()-1]);
        }
        else
        {
            // if it is known that a valid core entry has been seen, we can store the couple
            if(pCurrentList)
            {
                pCurrentList->push_back(CoreInfoItem(words[0].trimmed(), words[1].trimmed()));
            }
        }
    }
}

void InformationFetcher::fetchCpuInfo()
{
    // Create a process instance and let our system command run, fetching the results for future parsing
    QProcess fetchProcess;
    fetchProcess.start("sh", QStringList() << "-c" << "cat /proc/cpuinfo");
    fetchProcess.waitForFinished(-1);

    // Get the results and lineify them
    QString allResult = fetchProcess.readAllStandardOutput();
    QStringList lines = allResult.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    // Get the results semantically parsed and inserted/updated into model context
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

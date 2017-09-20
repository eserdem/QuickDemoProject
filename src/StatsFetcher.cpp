#include "StatsFetcher.h"

#include <QProcess>
#include <QStringList>
#include <QRegularExpression>
#include <QStandardItemModel>

StatsFetcher::StatsFetcher()
: m_pCoreStats(NULL)
{
}

StatsFetcher::~StatsFetcher()
{
}

void StatsFetcher::constructDataModelFromDatabse(const CoreStatDatabase& coreDb)
{
    if(NULL == m_pCoreStats)
    {
        m_pCoreStats = new QStandardItemModel(coreDb.size(), 5);
    }

    QMap<int, QVariant> mapRoles;
    int currentRow = 0;

    for(CoreStatDatabase::const_iterator listItr = coreDb.cbegin(); listItr != coreDb.cend(); ++listItr)
    {
        const SCPUUsage& usage = (*listItr);
        const QString name = "CPU " + QString::number(currentRow);

        mapRoles.insert( Qt::DisplayRole, name);
        mapRoles.insert( Qt::DecorationRole, usage.m_userMode);
        mapRoles.insert( Qt::EditRole, usage.m_kernelMode);
        mapRoles.insert( Qt::ToolTipRole, usage.m_other);
        mapRoles.insert( Qt::StatusTipRole, usage.m_idle);

        QStandardItem* pRowItem = new QStandardItem();

        m_pCoreStats->setItem(currentRow, pRowItem);
        m_pCoreStats->setItemData(pRowItem->index(), mapRoles);

        currentRow++;
    }
}

void StatsFetcher::buildDatabaseFromCoreInfo(const QStringList& lines, CoreStatDatabase& coreDb)
{
    const int lineCount = lines.size();

    for(int i = 0; i<lineCount; ++i)
    {
        if(!lines[i].startsWith("cpu"))
        {
            // We are only interested in cpu performance entries, rest is skipped
            continue;
        }

        if(lines[i].startsWith("cpu "))
        {
            // We skip total usage information for now
            continue;
        }

        const QStringList words = lines[i].split(" ", QString::SkipEmptyParts);

        if(words.size() < 8)
        {
            // Something is wrong with the fetch
            continue;
        }

        QString cpuString = words[0];
        cpuString.remove(0, 3);
        const int cpuId = cpuString.toInt();

        const int usageUserNormal = words[1].toInt();
        const int usageUserNice   = words[2].toInt();
        const int usageKernel     = words[3].toInt();
        const int usageIdle       = words[4].toInt();
        const int usageIOWait     = words[5].toInt();
        const int usageIrq        = words[6].toInt();
        const int usageSoftIrq    = words[7].toInt();

        const int totalSum = usageUserNormal + usageUserNice + usageKernel + usageIdle + usageIOWait + usageIrq + usageSoftIrq;

        SCPUUsage& usage = coreDb[cpuId];
        usage.m_idle = (usageIdle * 100.f) / totalSum;
        usage.m_other = ((usageIOWait + usageIrq + usageSoftIrq) * 100.f) / totalSum;
        usage.m_kernelMode = (usageKernel * 100.f) / totalSum;
        usage.m_userMode = ((usageUserNormal + usageUserNice) * 100.f) / totalSum;
    }
}

void StatsFetcher::startFetchingInfo()
{
    QProcess fetchProcess;
    fetchProcess.start("sh", QStringList() << "-c" << "cat /proc/stat");
    fetchProcess.waitForFinished(-1);

    QString allResult = fetchProcess.readAllStandardOutput();
    QStringList lines = allResult.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    CoreStatDatabase db;
    buildDatabaseFromCoreInfo(lines, db);
    constructDataModelFromDatabse(db);
}

QStandardItemModel* StatsFetcher::getCoreStats() const
{
    return m_pCoreStats;
}

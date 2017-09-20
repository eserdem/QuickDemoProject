#include "StatsFetcher.h"

#include <QProcess>
#include <QStringList>
#include <QRegularExpression>
#include <QStandardItemModel>
#include <QTimer>

StatsFetcher::StatsFetcher(QObject*)
: m_pCoreStats(NULL)
, m_pTimer(NULL)
{
}

StatsFetcher::~StatsFetcher()
{
    stopFetchingInfo();
}

void StatsFetcher::constructDataModelFromDatabase(const CoreStatDatabase& coreDb)
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

        mapRoles.insert( Qt::DisplayRole, usage.m_userMode);
        mapRoles.insert( Qt::DecorationRole, usage.m_kernelMode);
        mapRoles.insert( Qt::ToolTipRole, usage.m_other);
        mapRoles.insert( Qt::StatusTipRole, usage.m_idle);

        QStandardItem* pRowItem = m_pCoreStats->item(currentRow);

        if(NULL == pRowItem)
        {
            pRowItem = new QStandardItem();
            m_pCoreStats->setItem(currentRow, pRowItem);
        }

        m_pCoreStats->setItemData(pRowItem->index(), mapRoles);
        currentRow++;
    }
}

void StatsFetcher::onCyclicUpdate()
{
    CoreStatDatabase db;
    QStringList lines;

    fetchStats(lines);
    buildDatabaseFromCoreInfo(lines, db);
    constructDataModelFromDatabase(db);
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

void StatsFetcher::fetchStats(QStringList &lines)
{
    QProcess fetchProcess;
    fetchProcess.start("sh", QStringList() << "-c" << "cat /proc/stat");
    fetchProcess.waitForFinished(-1);

    QString allResult = fetchProcess.readAllStandardOutput();
    lines = allResult.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
}

void StatsFetcher::startFetchingInfo()
{
    // we yield first run synchronously here to make sure data is available immediately
    onCyclicUpdate();

    if(NULL == m_pTimer)
    {
        m_pTimer = new QTimer();

        QObject::connect(m_pTimer, &QTimer::timeout, this, &StatsFetcher::onCyclicUpdate);

        m_pTimer->start(1000);
    }
}

void StatsFetcher::stopFetchingInfo()
{
    if(m_pTimer)
    {
        m_pTimer->stop();
        delete m_pTimer;
        m_pTimer = NULL;
    }
}

QStandardItemModel* StatsFetcher::getCoreStats() const
{
    return m_pCoreStats;
}

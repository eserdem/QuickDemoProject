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
    // If necessary, cancel cyclic updates
    stopFetchingInfo();
}

void StatsFetcher::constructDataModelFromDatabase(const CoreStatDatabase& coreDb)
{
    // If we failed to fetch any data, we can skip data model creation as well
    if(0 == coreDb.size())
    {
        return;
    }

    // If it is the first time we are called, prepare model object
    if(NULL == m_pCoreStats)
    {
        m_pCoreStats = new QStandardItemModel(coreDb.size(), 5);
    }

    QMap<int, QVariant> mapRoles;
    int currentRow = 0;

    // Loop over availble items in database and establish data insertions/updates
    for(CoreStatDatabase::const_iterator listItr = coreDb.cbegin(); listItr != coreDb.cend(); ++listItr)
    {
        const SCPUUsage& usage = (*listItr);

        // Prepare role-data binding
        mapRoles.insert( Qt::DisplayRole, usage.m_userMode);
        mapRoles.insert( Qt::DecorationRole, usage.m_kernelMode);
        mapRoles.insert( Qt::ToolTipRole, usage.m_other);
        mapRoles.insert( Qt::StatusTipRole, usage.m_idle);

        QStandardItem* pRowItem = m_pCoreStats->item(currentRow);

        // Only allocate a new item if this was not allocated yet. This will minimize UI load
        if(NULL == pRowItem)
        {
            pRowItem = new QStandardItem();
            m_pCoreStats->setItem(currentRow, pRowItem);
        }

        // Freshly or previously allocated, time to update data on model item
        m_pCoreStats->setItemData(pRowItem->index(), mapRoles);
        currentRow++;
    }
}

void StatsFetcher::onCyclicUpdate()
{
    // We will first get lines for cpu status and get databse prepared from then, finally updating the model
    CoreStatDatabase db;
    QStringList lines;

    fetchStats(lines);
    buildDatabaseFromCoreInfo(lines, db);
    constructDataModelFromDatabase(db);
}

void StatsFetcher::buildDatabaseFromCoreInfo(const QStringList& lines, CoreStatDatabase& coreDb)
{
    const int lineCount = lines.size();

    // For each line we will be processing the content
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

        // Find out which cpu we are processing now.
        QString cpuString = words[0];
        cpuString.remove(0, 3);
        const int cpuId = cpuString.toInt();

        // Adjust buffer size if necessary
        if(!(m_buffers[0].size() > cpuId))
        {
            for(int j=0; j<StatBufferSize; ++j)
            {
                m_buffers[j].resize(cpuId+1);
            }
        }

        // Cycle buffer ring
        for(int j=(StatBufferSize-1); j>0; --j)
        {
            m_buffers[j][cpuId] = m_buffers[j-1][cpuId];
        }

        // Easy reading helpers
        SCPUUsageRaw& thisCpuRawUsageCurrent = m_buffers[0][cpuId];
        const SCPUUsageRaw& thisCpuRawUsagePrev = m_buffers[StatBufferSize-1][cpuId];

        // Store current stats
        thisCpuRawUsageCurrent.m_idle = words[4].toInt();
        thisCpuRawUsageCurrent.m_other = words[5].toInt() + words[6].toInt() + words[7].toInt();
        thisCpuRawUsageCurrent.m_kernelMode = words[3].toInt();
        thisCpuRawUsageCurrent.m_userMode = words[1].toInt() + words[2].toInt();

        // Get Diff stats
        const int diffIdle = thisCpuRawUsageCurrent.m_idle - thisCpuRawUsagePrev.m_idle;
        const int diffOther = thisCpuRawUsageCurrent.m_other - thisCpuRawUsagePrev.m_other;
        const int diffKernel = thisCpuRawUsageCurrent.m_kernelMode - thisCpuRawUsagePrev.m_kernelMode;
        const int diffUser = thisCpuRawUsageCurrent.m_userMode - thisCpuRawUsagePrev.m_userMode;

        const int diffSum = diffIdle + diffOther + diffKernel + diffUser;

        // Prepare floating point values
        SCPUUsage& usage = coreDb[cpuId];
        usage.m_idle = (diffIdle * 100.f) / diffSum;
        usage.m_other = (diffOther * 100.f) / diffSum;
        usage.m_kernelMode = (diffKernel * 100.f) / diffSum;
        usage.m_userMode = (diffUser * 100.f) / diffSum;
    }
}

void StatsFetcher::fetchStats(QStringList &lines)
{
    // Prepare the process object to run linux system command fetching status data
    QProcess fetchProcess;
    fetchProcess.start("sh", QStringList() << "-c" << "cat /proc/stat");
    fetchProcess.waitForFinished(-1);

    // Linefy obtained result
    QString allResult = fetchProcess.readAllStandardOutput();
    lines = allResult.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
}

void StatsFetcher::startFetchingInfo()
{
    // we yield first run synchronously here to make sure data is available immediately
    onCyclicUpdate();

    // if not yet created, create timer and configure cyclic update schedule
    if(NULL == m_pTimer)
    {
        m_pTimer = new QTimer();
        QObject::connect(m_pTimer, &QTimer::timeout, this, &StatsFetcher::onCyclicUpdate);
        m_pTimer->start(StatsUpdateRateinMS);
    }
}

void StatsFetcher::stopFetchingInfo()
{
    // If not yet deallocated, stop the timer and deallocate timer object
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

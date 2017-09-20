#ifndef STATSFETCHER_H
#define STATSFETCHER_H

#include <QObject>
#include <QVector>
#include <QMap>

class QStandardItemModel;
class QStringList;
class QTimer;

// Fetches and real-time updates CPU load information for all available cores. Updated data is reflected to Model container
class StatsFetcher : public QObject
{
    Q_OBJECT

public:
    StatsFetcher(QObject* pParent = NULL);
    ~StatsFetcher();

    // Starts fetching cpu usage information prepares information data available. Data can be used after this point
    void startFetchingInfo();

    // Stops fetching cpu usage information
    void stopFetchingInfo();

    // Returns data model containing core stats data
    QStandardItemModel* getCoreStats() const;

private:
        // Global Configuration about how StatFetcher should behave
        enum StatsFetcherConfig
        {
            StatBufferSize      = 5,    // This parameter defines the size of buffer for smoothness of the visualization. Minimum value should be 2
            StatsUpdateRateinMS = 500   // Defines in Milliseconds how often cpu status is queried from the system
        };

private:
    struct SCPUUsage
    {
        SCPUUsage()
        {
            m_userMode = 0.f;
            m_kernelMode = 0.f;
            m_other = 0.f;
            m_idle = 100.f;
        }

        float m_userMode;
        float m_kernelMode;
        float m_other;
        float m_idle;
    };

    struct SCPUUsageRaw
    {
        SCPUUsageRaw()
        {
            m_userMode = 0;
            m_kernelMode = 0;
            m_other = 0;
            m_idle = 0;
        }

        qlonglong m_userMode;
        qlonglong m_kernelMode;
        qlonglong m_other;
        qlonglong m_idle;
    };

    typedef QMap<int, SCPUUsage>     CoreStatDatabase;

    // Internally used to build database container from provided lines of information
    void buildDatabaseFromCoreInfo(const QStringList& lines, CoreStatDatabase& coreDb);

    // Internally used to construct data model from provided database
    void constructDataModelFromDatabase(const CoreStatDatabase& coreDb);

    // Internally used to fetch cpu stats and fill in the lines container
    void fetchStats(QStringList& lines);

private slots:
    // Periodically called back to yield data update functionality
    void onCyclicUpdate();

private:
    QVector<SCPUUsageRaw>   m_buffers[StatBufferSize];
    QStandardItemModel*     m_pCoreStats;
    QTimer*                 m_pTimer;



};

#endif // STATSFETCHER_H

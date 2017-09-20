#ifndef STATSFETCHER_H
#define STATSFETCHER_H

#include <QObject>
#include <QVector>
#include <QMap>

class QStandardItemModel;
class QStringList;
class QTimer;

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
        enum StatsFetcherConfig
        {
            StatBufferSize = 5,
            StatsUpdateRateinMS = 500
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

    void buildDatabaseFromCoreInfo(const QStringList& lines, CoreStatDatabase& coreDb);
    void constructDataModelFromDatabase(const CoreStatDatabase& coreDb);
    void fetchStats(QStringList& lines);

private slots:
    void onCyclicUpdate();

private:
    QVector<SCPUUsageRaw>   m_buffers[StatBufferSize];
    QStandardItemModel*     m_pCoreStats;
    QTimer*                 m_pTimer;



};

#endif // STATSFETCHER_H

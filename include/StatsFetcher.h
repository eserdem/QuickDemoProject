#ifndef STATSFETCHER_H
#define STATSFETCHER_H

#include <QObject>
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

    typedef QMap<int, SCPUUsage>     CoreStatDatabase;

    void buildDatabaseFromCoreInfo(const QStringList& lines, CoreStatDatabase& coreDb);
    void constructDataModelFromDatabase(const CoreStatDatabase& coreDb);
    void fetchStats(QStringList& lines);

private slots:
    void onCyclicUpdate();

private:
    QStandardItemModel* m_pCoreStats;
    QTimer*             m_pTimer;



};

#endif // STATSFETCHER_H

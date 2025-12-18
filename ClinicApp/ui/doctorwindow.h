#ifndef DOCTORWINDOW_H
#define DOCTORWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class DoctorWindow : public QWidget {
    Q_OBJECT
public:
    explicit DoctorWindow(QString name, QWidget *parent = nullptr);

private:
    QString m_docName;
    QTabWidget *m_tabs;
    QTimer *m_refreshTimer;

    QTableWidget *m_queueTable;
    QTextEdit *m_patientHistory;
    QTextEdit *m_inputDiagnosis;
    QPushButton *m_btnFinish;
    QLabel *m_lblCurrentPatient;
    QTableWidget *m_scheduleTable;
    QPushButton *m_btnGenSchedule;

    void setupWorkspaceTab();
    void setupScheduleTab();

    void refreshSchedule();
    void refreshQueue();
    void loadPatientInfo(QString patientName);
};

#endif

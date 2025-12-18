#ifndef PATIENTWINDOW_H
#define PATIENTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QLabel>
#include <QTimer>

class PatientWindow : public QWidget {
    Q_OBJECT
public:
    explicit PatientWindow(QString name, QWidget *parent = nullptr);

private:
    QString m_name;
    QTabWidget *m_tabs;
    QTimer *m_refreshTimer;

    QTableWidget *m_docTable;
    QListWidget *m_slotsList;
    QLabel *m_lblSelectedDoc;

    QTableWidget *m_histTable;
    QTextEdit *m_recordDetails;

    void setupBookingTab();
    void setupHistoryTab();
    void refreshDoctors();
    void refreshHistory();
};

#endif

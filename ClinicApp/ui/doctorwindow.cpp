#include "doctorwindow.h"
#include "clinicmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QSplitter>
#include <QDate>

DoctorWindow::DoctorWindow(QString name, QWidget *parent) : QWidget(parent), m_docName(name) {
    setWindowTitle("АРМ Лікаря");
    resize(1000, 700);

    Doctor currentDoc = ClinicManager::get()->getDoctorDetails(name);

    QString titleText = currentDoc.getRole() + ": " + name + " (" + currentDoc.getShortInfo() + ")";

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *lblTitle = new QLabel(titleText);
    lblTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #ddd;");

    QPushButton *btnLogout = new QPushButton("ВИЙТИ");
    btnLogout->setFixedWidth(150);
    btnLogout->setStyleSheet("background-color: #555; border: 1px solid #777;");

    headerLayout->addWidget(lblTitle);
    headerLayout->addStretch();
    headerLayout->addWidget(btnLogout);

    mainLayout->addLayout(headerLayout);

    m_tabs = new QTabWidget(this);
    setupWorkspaceTab();
    setupScheduleTab();

    mainLayout->addWidget(m_tabs);

    connect(btnLogout, &QPushButton::clicked, [this](){
        this->close();
    });

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(5000);

    connect(m_refreshTimer, &QTimer::timeout, [this](){
        int currentRow = m_queueTable->currentRow();
        ClinicManager::get()->loadData();
        refreshQueue();
        refreshSchedule();
        if(currentRow >= 0 && currentRow < m_queueTable->rowCount()) {
            m_queueTable->selectRow(currentRow);
        }
    });
    m_refreshTimer->start();
}

void DoctorWindow::setupWorkspaceTab() {
    QWidget *page = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(page);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);

    QGroupBox *grpQueue = new QGroupBox("Черга записів на сьогодні");
    QVBoxLayout *l1 = new QVBoxLayout(grpQueue);

    m_queueTable = new QTableWidget(0, 3);
    m_queueTable->setHorizontalHeaderLabels({"Час/Дата", "ПІБ Пацієнта", "Статус"});
    m_queueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_queueTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_queueTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPushButton *btnRefresh = new QPushButton("Оновити список вручну");
    l1->addWidget(btnRefresh);
    l1->addWidget(m_queueTable);

    QWidget *rightWidget = new QWidget();
    QVBoxLayout *l2 = new QVBoxLayout(rightWidget);

    QGroupBox *grpInfo = new QGroupBox("Медична карта пацієнта");
    QVBoxLayout *infoLayout = new QVBoxLayout(grpInfo);
    m_patientHistory = new QTextEdit();
    m_patientHistory->setReadOnly(true);
    m_patientHistory->setPlaceholderText("Оберіть пацієнта зі списку зліва...");
    infoLayout->addWidget(m_patientHistory);

    QGroupBox *grpAction = new QGroupBox("Проведення прийому");
    QVBoxLayout *actionLayout = new QVBoxLayout(grpAction);

    m_lblCurrentPatient = new QLabel("Пацієнт не обраний");
    m_lblCurrentPatient->setStyleSheet("font-weight: bold; font-size: 14px; color: #4CAF50;");

    m_inputDiagnosis = new QTextEdit();
    m_inputDiagnosis->setPlaceholderText("Впишіть скарги, діагноз та призначення...");

    m_btnFinish = new QPushButton("ЗБЕРЕГТИ ЗАПИС");
    m_btnFinish->setStyleSheet("background-color: #d9534f; font-weight: bold; padding: 10px;");
    m_btnFinish->setEnabled(false);

    Doctor d = ClinicManager::get()->getDoctorDetails(m_docName);
    if (!d.canEditRecords()) {
        m_inputDiagnosis->setPlaceholderText("У вас немає прав для редагування.");
        m_inputDiagnosis->setReadOnly(true);
        m_btnFinish->setVisible(false);
    }

    actionLayout->addWidget(new QLabel("Пацієнт:"));
    actionLayout->addWidget(m_lblCurrentPatient);
    actionLayout->addWidget(new QLabel("Висновок:"));
    actionLayout->addWidget(m_inputDiagnosis);
    actionLayout->addWidget(m_btnFinish);

    l2->addWidget(grpInfo, 1);
    l2->addWidget(grpAction, 1);

    splitter->addWidget(grpQueue);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    layout->addWidget(splitter);
    m_tabs->addTab(page, "Робочий стіл (Прийом)");

    refreshQueue();

    connect(btnRefresh, &QPushButton::clicked, [this](){
        ClinicManager::get()->loadData();
        refreshQueue();
        QMessageBox::information(this, "Інфо", "Список оновлено!");
    });

    connect(m_queueTable, &QTableWidget::cellClicked, [this](int r, int){
        QString patName = m_queueTable->item(r, 1)->text();
        QString status = m_queueTable->item(r, 2)->text();

        loadPatientInfo(patName);

        Doctor d = ClinicManager::get()->getDoctorDetails(m_docName);
        bool hasRights = d.canEditRecords();

        if (status == "Очікує" && hasRights) {
            m_lblCurrentPatient->setText(patName);
            m_inputDiagnosis->setEnabled(true);
            m_btnFinish->setEnabled(true);
        } else {
            m_lblCurrentPatient->setText(patName + (hasRights ? " (Завершено)" : ""));
            m_inputDiagnosis->setPlaceholderText(hasRights ? "Прийом завершено." : "Немає прав.");
            m_inputDiagnosis->setEnabled(false);
            m_btnFinish->setEnabled(false);
        }
    });

    connect(m_btnFinish, &QPushButton::clicked, [this](){
        int r = m_queueTable->currentRow();
        if(r < 0) return;

        QString patName = m_queueTable->item(r, 1)->text();
        QString time = m_queueTable->item(r, 0)->text();
        QString diagnosis = m_inputDiagnosis->toPlainText();

        if(diagnosis.isEmpty()) {
            QMessageBox::warning(this, "Увага", "Введіть діагноз!"); return;
        }

        ClinicManager::get()->completeAppointment(m_docName, patName, time, diagnosis);

        QMessageBox::information(this, "Успіх", "Прийом завершено.");
        m_inputDiagnosis->clear();
        m_inputDiagnosis->setEnabled(false);
        m_btnFinish->setEnabled(false);
        refreshQueue();
    });
}

void DoctorWindow::setupScheduleTab() {
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);

    m_btnGenSchedule = new QPushButton("Оновити графік вручну");

    m_scheduleTable = new QTableWidget(0, 2);
    m_scheduleTable->setHorizontalHeaderLabels({"Час слоту", "Статус"});
    m_scheduleTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_scheduleTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(new QLabel("Вільні слоти:"));
    layout->addWidget(m_btnGenSchedule);
    layout->addWidget(m_scheduleTable);

    m_tabs->addTab(page, "Мій розклад");
    refreshSchedule();
    connect(m_btnGenSchedule, &QPushButton::clicked, [this](){
        ClinicManager::get()->loadData();
        refreshSchedule();
    });
}

void DoctorWindow::loadPatientInfo(QString patientName) {
    Patient p = ClinicManager::get()->getPatientDetails(patientName);
    QString html = "<h3>Історія: " + patientName + "</h3><ul>";
    for(auto &app : p.getHistory()) {
        if(app.isCompleted()) {
            html += QString("<li><b>%1</b> (%2): %3</li>")
            .arg(app.getTime(), app.getDoctorName(), app.getDiagnosis());
        }
    }
    html += "</ul>";
    if(p.getHistory().isEmpty()) html += "<p>Записів немає.</p>";
    m_patientHistory->setHtml(html);
}

void DoctorWindow::refreshQueue() {
    Doctor d = ClinicManager::get()->getDoctorDetails(m_docName);
    m_queueTable->setRowCount(0);
    for(auto &app : d.getAppointments()) {
        int r = m_queueTable->rowCount();
        m_queueTable->insertRow(r);
        m_queueTable->setItem(r, 0, new QTableWidgetItem(app.getTime()));
        m_queueTable->setItem(r, 1, new QTableWidgetItem(app.getPatientName()));

        QString statusStr = app.isCompleted() ? "Завершено" : "Очікує";
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusStr);
        statusItem->setForeground(app.isCompleted() ? Qt::gray : Qt::darkGreen);
        m_queueTable->setItem(r, 2, statusItem);
    }
}

void DoctorWindow::refreshSchedule() {
    Doctor d = ClinicManager::get()->getDoctorDetails(m_docName);
    m_scheduleTable->setRowCount(0);
    QStringList allSlots = {"09:00", "09:30", "10:00", "10:30", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};

    for(const auto &slot : allSlots) {
        int r = m_scheduleTable->rowCount();
        m_scheduleTable->insertRow(r);
        m_scheduleTable->setItem(r, 0, new QTableWidgetItem(slot));

        bool isFree = d.getSlots().contains(slot);
        QTableWidgetItem *statusItem;
        if(isFree) {
            statusItem = new QTableWidgetItem("Вільно");
            statusItem->setForeground(QBrush(QColor("#4CAF50")));
        } else {
            statusItem = new QTableWidgetItem("Зайнято");
            statusItem->setForeground(QBrush(QColor("#d9534f")));
        }
        statusItem->setFlags(statusItem->flags() ^ Qt::ItemIsEditable);
        m_scheduleTable->setItem(r, 1, statusItem);
    }
}

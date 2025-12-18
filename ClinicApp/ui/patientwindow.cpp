#include "patientwindow.h"
#include "clinicmanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QGroupBox>
#include <QSplitter>

PatientWindow::PatientWindow(QString name, QWidget *parent) : QWidget(parent), m_name(name) {
    setWindowTitle("Електронна реєстратура");
    resize(900, 600);
    Patient currentPat = ClinicManager::get()->getPatientDetails(name);
    QString titleText = currentPat.getRole() + ": " + name + " (" + currentPat.getShortInfo() + ")";
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
    setupBookingTab();
    setupHistoryTab();

    mainLayout->addWidget(m_tabs);

    connect(btnLogout, &QPushButton::clicked, [this](){
        this->close();
    });

    m_refreshTimer = new QTimer(this);
    m_refreshTimer->setInterval(5000);

    connect(m_refreshTimer, &QTimer::timeout, [this, lblTitle](){
        int currentDocRow = m_docTable->currentRow();
        ClinicManager::get()->loadData();

        Patient updatedPat = ClinicManager::get()->getPatientDetails(m_name);
        lblTitle->setText(updatedPat.getRole() + ": " + m_name + " (" + updatedPat.getShortInfo() + ")");

        refreshDoctors();
        refreshHistory();

        if(currentDocRow >= 0 && currentDocRow < m_docTable->rowCount()) {
            m_docTable->selectRow(currentDocRow);
        }
    });

    m_refreshTimer->start();
}

void PatientWindow::setupBookingTab() {
    QWidget *bookingPage = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(bookingPage);

    QGroupBox *grpDoctors = new QGroupBox("1. Оберіть лікаря");
    QVBoxLayout *docLayout = new QVBoxLayout(grpDoctors);

    QPushButton *btnRefreshDocs = new QPushButton("Оновити список лікарів");
    btnRefreshDocs->setStyleSheet("background-color: #5bc0de; color: white; font-weight: normal;");

    m_docTable = new QTableWidget(0, 2);
    m_docTable->setHorizontalHeaderLabels({"ПІБ Лікаря", "Спеціальність"});
    m_docTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_docTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_docTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    docLayout->addWidget(btnRefreshDocs);
    docLayout->addWidget(m_docTable);

    QGroupBox *grpSlots = new QGroupBox("2. Доступний час");
    QVBoxLayout *slotLayout = new QVBoxLayout(grpSlots);

    m_lblSelectedDoc = new QLabel("Лікар не обраний");
    m_lblSelectedDoc->setStyleSheet("font-weight: bold; color: #4CAF50;");

    m_slotsList = new QListWidget();

    QPushButton *btnBook = new QPushButton("Підтвердити запис");
    btnBook->setMinimumHeight(40);

    slotLayout->addWidget(new QLabel("Ви записуєтесь до:"));
    slotLayout->addWidget(m_lblSelectedDoc);
    slotLayout->addWidget(m_slotsList);
    slotLayout->addSpacing(10);
    slotLayout->addWidget(btnBook);

    layout->addWidget(grpDoctors, 2);
    layout->addWidget(grpSlots, 1);

    m_tabs->addTab(bookingPage, "Запис на прийом");

    refreshDoctors();

    connect(btnRefreshDocs, &QPushButton::clicked, [this](){
        ClinicManager::get()->loadData();
        refreshDoctors();
        QMessageBox::information(this, "Інфо", "Список оновлено!");
    });

    connect(m_docTable, &QTableWidget::cellClicked, [this](int r, int){
        m_slotsList->clear();
        QString docName = m_docTable->item(r, 0)->text();
        QString spec = m_docTable->item(r, 1)->text();
        m_lblSelectedDoc->setText(docName + "\n(" + spec + ")");

        ClinicManager::get()->loadData();
        Doctor d = ClinicManager::get()->getDoctorDetails(docName);
        if(d.getSlots().isEmpty()) {
            m_slotsList->addItem("Немає вільних місць");
        } else {
            for(auto &s : d.getSlots()) m_slotsList->addItem(s);
        }
    });

    connect(btnBook, &QPushButton::clicked, [this](){
        int r = m_docTable->currentRow();
        if(r < 0) { QMessageBox::warning(this, "!", "Оберіть лікаря."); return; }

        if(!m_slotsList->currentItem() || m_slotsList->currentItem()->text() == "Немає вільних місць") {
            QMessageBox::warning(this, "!", "Оберіть час."); return;
        }

        QString doc = m_docTable->item(r, 0)->text();
        QString time = m_slotsList->currentItem()->text();

        if(ClinicManager::get()->bookAppointment(doc, m_name, time)) {
            QMessageBox::information(this, "Успіх", "Запис успішно створено!");
            m_slotsList->takeItem(m_slotsList->currentRow());
            refreshHistory();
            m_tabs->setCurrentIndex(1);
        } else {
            QMessageBox::warning(this, "Помилка", "Цей час вже зайнято!");
            ClinicManager::get()->loadData();
        }
    });
}

void PatientWindow::setupHistoryTab() {
    QWidget *histPage = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(histPage);

    QSplitter *splitter = new QSplitter(Qt::Vertical);

    QGroupBox *grpList = new QGroupBox("Історія відвідувань");
    QVBoxLayout *l1 = new QVBoxLayout(grpList);
    m_histTable = new QTableWidget(0, 3);
    m_histTable->setHorizontalHeaderLabels({"Дата/Час", "Лікар", "Статус"});
    m_histTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_histTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_histTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    l1->addWidget(m_histTable);

    QGroupBox *grpDetails = new QGroupBox("Деталі запису");
    QVBoxLayout *l2 = new QVBoxLayout(grpDetails);
    m_recordDetails = new QTextEdit();
    m_recordDetails->setReadOnly(true);
    m_recordDetails->setPlaceholderText("Оберіть запис для перегляду...");
    l2->addWidget(m_recordDetails);

    splitter->addWidget(grpList);
    splitter->addWidget(grpDetails);

    layout->addWidget(splitter);

    QPushButton *btnRefresh = new QPushButton("Оновити дані вручну");
    layout->addWidget(btnRefresh);

    m_tabs->addTab(histPage, "Моя медична картка");

    refreshHistory();

    connect(btnRefresh, &QPushButton::clicked, [this](){
        ClinicManager::get()->loadData();
        refreshHistory();
    });

    connect(m_histTable, &QTableWidget::cellClicked, [this](int r, int){
        Patient p = ClinicManager::get()->getPatientDetails(m_name);
        if(r < p.getHistory().size()) {
            auto app = p.getHistory()[r];
            QString html = QString(
                               "<h2>Запис: %1</h2>"
                               "<p><b>Лікар:</b> %2</p>"
                               "<hr>"
                               "<h3>Висновок:</h3>"
                               "<p style='color: #4CAF50'>%3</p>"
                               ).arg(app.getTime(), app.getDoctorName(),
                                    app.isCompleted() ? app.getDiagnosis() : "<i>Прийом не завершено</i>");
            m_recordDetails->setHtml(html);
        }
    });
}

void PatientWindow::refreshDoctors() {
    m_docTable->setRowCount(0);
    for(auto &d : ClinicManager::get()->getAllDoctors()) {
        int r = m_docTable->rowCount(); m_docTable->insertRow(r);
        m_docTable->setItem(r, 0, new QTableWidgetItem(d.getName()));
        m_docTable->setItem(r, 1, new QTableWidgetItem(d.getSpecialty()));
    }
}

void PatientWindow::refreshHistory() {
    m_histTable->setRowCount(0);
    Patient p = ClinicManager::get()->getPatientDetails(m_name);
    for(auto &a : p.getHistory()) {
        int r = m_histTable->rowCount(); m_histTable->insertRow(r);
        m_histTable->setItem(r, 0, new QTableWidgetItem(a.getTime()));
        m_histTable->setItem(r, 1, new QTableWidgetItem(a.getDoctorName()));

        QTableWidgetItem *status = new QTableWidgetItem(a.isCompleted() ? "Завершено" : "Заплановано");
        status->setForeground(a.isCompleted() ? Qt::green : QColor("orange"));
        m_histTable->setItem(r, 2, status);
    }
}

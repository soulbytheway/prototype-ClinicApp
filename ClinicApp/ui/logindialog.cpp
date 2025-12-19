#include "logindialog.h"
#include "clinicmanager.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Вхід у систему");
    setFixedSize(350, 450);
    setupUI();
}

void LoginDialog::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    m_tabs = new QTabWidget(this);

    QWidget *tabLogin = new QWidget();
    QVBoxLayout *lLogin = new QVBoxLayout(tabLogin);

    m_loginL = new QLineEdit(); m_loginL->setPlaceholderText("Логін");
    m_passL = new QLineEdit(); m_passL->setPlaceholderText("Пароль"); m_passL->setEchoMode(QLineEdit::Password);
    QPushButton *btnLogin = new QPushButton("УВІЙТИ");

    lLogin->addStretch();
    lLogin->addWidget(new QLabel("Логін:")); lLogin->addWidget(m_loginL);
    lLogin->addWidget(new QLabel("Пароль:")); lLogin->addWidget(m_passL);
    lLogin->addSpacing(20);
    lLogin->addWidget(btnLogin);
    lLogin->addStretch();

    QWidget *tabReg = new QWidget();
    QVBoxLayout *lReg = new QVBoxLayout(tabReg);

    m_nameR = new QLineEdit(); m_nameR->setPlaceholderText("ПІБ (напр. Іванов І.І.)");
    m_loginR = new QLineEdit(); m_loginR->setPlaceholderText("Придумайте логін");
    m_passR = new QLineEdit(); m_passR->setPlaceholderText("Пароль"); m_passR->setEchoMode(QLineEdit::Password);
    m_roleR = new QComboBox(); m_roleR->addItems({"Пацієнт", "Лікар"});
    m_specR = new QLineEdit(); m_specR->setPlaceholderText("Спеціальність (тільки для лікаря)");
    m_specR->setVisible(false);

    QPushButton *btnReg = new QPushButton("ЗАРЕЄСТРУВАТИСЯ");
    btnReg->setStyleSheet("background-color: #4CAF50;");

    lReg->addWidget(new QLabel("Роль:")); lReg->addWidget(m_roleR);
    lReg->addWidget(new QLabel("ПІБ:")); lReg->addWidget(m_nameR);
    lReg->addWidget(new QLabel("Логін:")); lReg->addWidget(m_loginR);
    lReg->addWidget(new QLabel("Пароль:")); lReg->addWidget(m_passR);
    QLabel *lblSpec = new QLabel("Спеціальність:");
    lblSpec->setVisible(false);
    lReg->addWidget(lblSpec); lReg->addWidget(m_specR);
    lReg->addSpacing(20);
    lReg->addWidget(btnReg);
    lReg->addStretch();

    m_tabs->addTab(tabLogin, "Вхід");
    m_tabs->addTab(tabReg, "Реєстрація");
    mainLayout->addWidget(m_tabs);

    connect(btnLogin, &QPushButton::clicked, this, &LoginDialog::onLogin);
    connect(btnReg, &QPushButton::clicked, this, &LoginDialog::onRegister);

    connect(m_roleR, &QComboBox::currentTextChanged, [=](const QString &text){
        bool isDoc = (text == "Лікар");
        m_specR->setVisible(isDoc);
        lblSpec->setVisible(isDoc);
    });
}

void LoginDialog::onLogin() {
    QString role = ClinicManager::get()->loginUser(m_loginL->text(), m_passL->text(), m_name);
    if(role.isEmpty()) {
        QMessageBox::warning(this, "Помилка", "Невірний логін або пароль");
    } else {
        m_role = (role == "doctor") ? "doctor" : "patient";
        accept();
    }
}

void LoginDialog::onRegister() {
    QString role = (m_roleR->currentText() == "Лікар") ? "doctor" : "patient";
    QString name = m_nameR->text();
    QString login = m_loginR->text();
    QString pass = m_passR->text();
    QString spec = m_specR->text();

    if(name.isEmpty() || login.isEmpty() || pass.isEmpty()) {
        QMessageBox::warning(this, "!", "Заповніть всі поля"); return;
    }
    if(role == "doctor" && !login.contains("doc")) {
        QMessageBox::warning(this, "!", "Логін лікаря має містити 'doc'"); return;
    }

    if(ClinicManager::get()->registerUser(name, login, pass, role, spec)) {
        QMessageBox::information(this, "Успіх", "Акаунт створено! Перейдіть на вкладку Вхід.");
        m_tabs->setCurrentIndex(0);
    } else {
        QMessageBox::warning(this, "Помилка", "Такий логін вже існує!");
    }
}



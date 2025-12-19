#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTabWidget>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getRole() const { return m_role; }
    QString getName() const { return m_name; }

private slots:
    void onLogin();
    void onRegister();

private:
    QTabWidget *m_tabs;
    QLineEdit *m_loginL, *m_passL;
    QLineEdit *m_nameR, *m_loginR, *m_passR, *m_specR;
    QComboBox *m_roleR;

    QString m_role;
    QString m_name;

    void setupUI();
};

#endif

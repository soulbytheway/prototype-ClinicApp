#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

class User {
protected:
    QString m_name;
    QString m_login;
    QString m_password;

public:
    User();
    User(QString name, QString login, QString pass);
    virtual ~User();
    QString getName() const;
    QString getLogin() const;
    bool checkPassword(QString input) const;
    virtual QJsonObject toJson() const = 0;
    virtual QString getRole() const = 0;
    virtual QString getShortInfo() const = 0;
    virtual bool canEditRecords() const;
};

#endif

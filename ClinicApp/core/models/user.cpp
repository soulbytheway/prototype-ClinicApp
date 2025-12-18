#include "user.h"

User::User() {}

User::User(QString name, QString login, QString pass)
    : m_name(name), m_login(login), m_password(pass) {}

User::~User() {}

QString User::getName() const { return m_name; }
QString User::getLogin() const { return m_login; }
bool User::checkPassword(QString input) const { return m_password == input; }

bool User::canEditRecords() const { return false; }

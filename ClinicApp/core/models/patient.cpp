#include "patient.h"
#include <QJsonArray>

Patient::Patient() : User() {}
Patient::Patient(QString name, QString login, QString pass) : User(name, login, pass) {}

QList<Appointment>& Patient::getHistory() { return m_history; }
void Patient::addRecord(const Appointment &app) { m_history.append(app); }

QString Patient::getRole() const { return "Пацієнт"; }

QString Patient::getShortInfo() const {
    return "Записів в історії: " + QString::number(m_history.size());
}

QJsonObject Patient::toJson() const {
    QJsonArray histArr;
    for(const auto &a : m_history) histArr.append(a.toJson());
    return {
        {"name", m_name}, {"login", m_login}, {"pass", m_password}, {"history", histArr}
    };
}

Patient Patient::fromJson(const QJsonObject &json) {
    Patient p(json["name"].toString(), json["login"].toString(), json["pass"].toString());
    for(const auto &v : json["history"].toArray()) p.m_history.append(Appointment::fromJson(v.toObject()));
    return p;
}

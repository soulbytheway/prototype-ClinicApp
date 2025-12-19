#include "doctor.h"
#include <QJsonArray>
#include <QJsonObject>

Doctor::Doctor() : User() {}

Doctor::Doctor(QString name, QString login, QString pass, QString specialty)
    : User(name, login, pass), m_specialty(specialty) {}

QString Doctor::getSpecialty() const { return m_specialty; }
QList<QString> Doctor::getSlots() const { return m_availableSlots; }
QList<Appointment>& Doctor::getAppointments() { return m_appointments; }

void Doctor::setSlots(const QList<QString> &newSlots) { m_availableSlots = newSlots; }

bool Doctor::reserveSlot(QString time) {
    if(m_availableSlots.contains(time)) {
        m_availableSlots.removeOne(time);
        return true;
    }
    return false;
}

void Doctor::addAppointment(const Appointment &app) { m_appointments.append(app); }

QString Doctor::getRole() const { return "Лікар"; }

QString Doctor::getShortInfo() const {
    return "Спеціальність: " + m_specialty;
}

bool Doctor::canEditRecords() const { return true; }

QJsonObject Doctor::toJson() const {
    QJsonObject json;
    json["name"] = m_name;
    json["login"] = m_login;
    json["pass"] = m_password;
    json["spec"] = m_specialty;

    QJsonArray slotsArr;
    for(const auto &s : m_availableSlots) slotsArr.append(s);
    json["slots"] = slotsArr;

    QJsonArray appArr;
    for(const auto &a : m_appointments) appArr.append(a.toJson());
    json["apps"] = appArr;

    return json;
}

Doctor Doctor::fromJson(const QJsonObject &json) {
    Doctor d(
        json["name"].toString(),
        json["login"].toString(),
        json["pass"].toString(),
        json["spec"].toString()
        );
    QJsonArray slotsArr = json["slots"].toArray();
    for(const auto &v : slotsArr) d.m_availableSlots.append(v.toString());

    QJsonArray appArr = json["apps"].toArray();
    for(const auto &v : appArr) d.m_appointments.append(Appointment::fromJson(v.toObject()));

    return d;
}

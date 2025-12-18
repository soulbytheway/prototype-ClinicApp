#include "appointment.h"

Appointment::Appointment() {}

Appointment::Appointment(QString id, QString doc, QString pat, QString time)
    : m_id(id), m_doctorName(doc), m_patientName(pat), m_time(time), m_isCompleted(false)
{
}

QString Appointment::getId() const { return m_id; }
QString Appointment::getDoctorName() const { return m_doctorName; }
QString Appointment::getPatientName() const { return m_patientName; }
QString Appointment::getTime() const { return m_time; }
QString Appointment::getDiagnosis() const { return m_diagnosis; }
bool Appointment::isCompleted() const { return m_isCompleted; }

void Appointment::complete(QString diagnosis) {
    m_diagnosis = diagnosis;
    m_isCompleted = true;
}

QJsonObject Appointment::toJson() const {
    return {
        {"id", m_id}, {"doc", m_doctorName}, {"pat", m_patientName},
        {"time", m_time}, {"diag", m_diagnosis}, {"completed", m_isCompleted}
    };
}

Appointment Appointment::fromJson(const QJsonObject &json) {
    Appointment a;
    a.m_id = json["id"].toString();
    a.m_doctorName = json["doc"].toString();
    a.m_patientName = json["pat"].toString();
    a.m_time = json["time"].toString();
    a.m_diagnosis = json["diag"].toString();
    a.m_isCompleted = json["completed"].toBool();
    return a;
}

#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <QString>
#include <QJsonObject>

class Appointment {
private:
    QString m_id;
    QString m_doctorName;
    QString m_patientName;
    QString m_time;
    QString m_diagnosis;
    bool m_isCompleted;

public:
    Appointment();
    Appointment(QString id, QString doc, QString pat, QString time);

    QString getId() const;
    QString getDoctorName() const;
    QString getPatientName() const;
    QString getTime() const;
    QString getDiagnosis() const;
    bool isCompleted() const;

    void complete(QString diagnosis);

    QJsonObject toJson() const;
    static Appointment fromJson(const QJsonObject &json);
};

#endif

#ifndef PATIENT_H
#define PATIENT_H

#include "user.h"
#include "appointment.h"
#include <QList>

class Patient : public User {
private:
    QList<Appointment> m_history;

public:
    Patient();
    Patient(QString name, QString login, QString pass);

    QList<Appointment>& getHistory();
    void addRecord(const Appointment& app);
    QJsonObject toJson() const override;
    static Patient fromJson(const QJsonObject &json);

    QString getRole() const override;
    QString getShortInfo() const override;
};

#endif

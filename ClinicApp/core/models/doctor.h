#ifndef DOCTOR_H
#define DOCTOR_H

#include "user.h"
#include "appointment.h"
#include <QList>
#include <QString>

class Doctor : public User {
private:
    QString m_specialty;
    QList<QString> m_availableSlots;
    QList<Appointment> m_appointments;

public:
    Doctor();
    Doctor(QString name, QString login, QString pass, QString specialty);

    QString getSpecialty() const;
    QList<QString> getSlots() const;
    QList<Appointment>& getAppointments();
    void setSlots(const QList<QString>& newSlots);
    bool reserveSlot(QString time);
    void addAppointment(const Appointment& app);

    QJsonObject toJson() const override;
    static Doctor fromJson(const QJsonObject &json);

    QString getRole() const override;
    QString getShortInfo() const override;
    bool canEditRecords() const override;
};

#endif

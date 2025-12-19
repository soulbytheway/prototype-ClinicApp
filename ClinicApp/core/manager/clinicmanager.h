#ifndef CLINICMANAGER_H
#define CLINICMANAGER_H

#include <QList>
#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include "doctor.h"
#include "patient.h"

class ClinicManager
{
private:
    static ClinicManager* instance;

    QList<Doctor> m_doctors;
    QList<Patient> m_patients;

    const QString m_filename = "clinic_data.json";

    ClinicManager();
    void saveData();

public:
    static ClinicManager* get();

    void loadData();

    bool registerUser(QString name, QString login, QString pass, QString role, QString specialty = "");
    QString loginUser(QString login, QString pass, QString &outName);

    QList<Doctor> getAllDoctors();
    Doctor getDoctorDetails(QString name);
    Patient getPatientDetails(QString name);

    bool bookAppointment(QString docName, QString patName, QString time);
    void completeAppointment(QString docName, QString patName, QString time, QString diagnosis);
};

#endif

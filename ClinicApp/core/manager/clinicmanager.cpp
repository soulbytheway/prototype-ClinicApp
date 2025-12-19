#include "clinicmanager.h"
#include <QDebug>
#include <QDateTime>

ClinicManager* ClinicManager::instance = nullptr;

ClinicManager::ClinicManager() {
    loadData();
}

ClinicManager* ClinicManager::get() {
    if(!instance) instance = new ClinicManager();
    return instance;
}


void ClinicManager::loadData() {
    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "File not found. Starting with empty DB.";
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject root = doc.object();

    m_doctors.clear();
    m_patients.clear();

    QJsonArray docArray = root["doctors"].toArray();
    for(const auto &v : docArray) {
        m_doctors.append(Doctor::fromJson(v.toObject()));
    }

    QJsonArray patArray = root["patients"].toArray();
    for(const auto &v : patArray) {
        m_patients.append(Patient::fromJson(v.toObject()));
    }

    file.close();
}

void ClinicManager::saveData() {
    QJsonObject root;

    QJsonArray docArray;
    for(const auto &d : m_doctors) docArray.append(d.toJson());
    root["doctors"] = docArray;

    QJsonArray patArray;
    for(const auto &p : m_patients) patArray.append(p.toJson());
    root["patients"] = patArray;

    QJsonDocument doc(root);
    QFile file(m_filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}


bool ClinicManager::registerUser(QString name, QString login, QString pass, QString role, QString specialty) {
    for(const auto &d : m_doctors) if(d.getLogin() == login) return false;
    for(const auto &p : m_patients) if(p.getLogin() == login) return false;

    if (role == "doctor") {
        Doctor newDoc(name, login, pass, specialty);
        newDoc.setSlots({"09:00", "09:30", "10:00", "10:30", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"});
        m_doctors.append(newDoc);
    } else {
        Patient newPat(name, login, pass);
        m_patients.append(newPat);
    }

    saveData();
    return true;
}

QString ClinicManager::loginUser(QString login, QString pass, QString &outName) {
    for(const auto &d : m_doctors) {
        if(d.getLogin() == login && d.checkPassword(pass)) {
            outName = d.getName();
            return "doctor";
        }
    }
    for(const auto &p : m_patients) {
        if(p.getLogin() == login && p.checkPassword(pass)) {
            outName = p.getName();
            return "patient";
        }
    }
    return "";
}

QList<Doctor> ClinicManager::getAllDoctors() { return m_doctors; }

Doctor ClinicManager::getDoctorDetails(QString name) {
    for(const auto &d : m_doctors) if(d.getName() == name) return d;
    return Doctor();
}

Patient ClinicManager::getPatientDetails(QString name) {
    for(const auto &p : m_patients) if(p.getName() == name) return p;
    return Patient();
}

bool ClinicManager::bookAppointment(QString docName, QString patName, QString time) {
    int docIndex = -1;
    for(int i = 0; i < m_doctors.size(); ++i) {
        if(m_doctors[i].getName() == docName) {
            docIndex = i;
            break;
        }
    }

    int patIndex = -1;
    for(int i = 0; i < m_patients.size(); ++i) {
        if(m_patients[i].getName() == patName) {
            patIndex = i;
            break;
        }
    }

    if(docIndex == -1 || patIndex == -1) return false;

    if(!m_doctors[docIndex].reserveSlot(time)) return false;

    QString id = QString::number(QDateTime::currentMSecsSinceEpoch());

    QString currentData = QDate::currentDate().toString("dd.MM.yyyy");
    QString fullDateTime = currentData + " " + time;

    Appointment app(id, docName, patName, fullDateTime);

    m_doctors[docIndex].addAppointment(app);
    m_patients[patIndex].addRecord(app);

    saveData();
    return true;
}

void ClinicManager::completeAppointment(QString docName, QString patName, QString time, QString diagnosis) {
    for(auto &d : m_doctors) {
        if(d.getName() == docName) {
            auto &apps = d.getAppointments();
            for(auto &a : apps) {
                if(a.getTime() == time && a.getPatientName() == patName) a.complete(diagnosis);
            }
        }
    }
    for(auto &p : m_patients) {
        if(p.getName() == patName) {
            auto &hist = p.getHistory();
            for(auto &a : hist) {
                if(a.getTime() == time && a.getDoctorName() == docName) a.complete(diagnosis);
            }
        }
    }
    saveData();
}

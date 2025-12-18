#include <QApplication>
#include "logindialog.h"
#include "doctorwindow.h"
#include "patientwindow.h"
#include "stylesheet.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    a.setStyleSheet(StyleHelper::getGlobalStyle());

    while (true) {
        LoginDialog login;

        if(login.exec() != QDialog::Accepted) {
            break;
        }

        QWidget *mainWindow = nullptr;

        if(login.getRole() == "doctor") {
            mainWindow = new DoctorWindow(login.getName());
        } else {
            mainWindow = new PatientWindow(login.getName());
        }

        if (mainWindow) {
            mainWindow->setAttribute(Qt::WA_DeleteOnClose);
            mainWindow->show();

            QEventLoop loop;
            QObject::connect(mainWindow, &QWidget::destroyed, &loop, &QEventLoop::quit);
            loop.exec();
        }
    }

    return 0;
}

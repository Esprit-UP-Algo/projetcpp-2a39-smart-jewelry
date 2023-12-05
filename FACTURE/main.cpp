#include "menu.h"
#include "ui_menu.h"
//#include "mainwindow_f.h"
#include "mainwindow_fac.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include "facture.h"
#include "mailer.h"
#include <QDebug>
#include "Sms.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test=c.createconnect();
   // MainWindow w;
  //  MainWindow_fac fact;
  menu m;

  // mainwindow_fac f;
    if(test)
    {m.show();

        QMessageBox::information(nullptr, QObject::tr("database is open"),
                    QObject::tr("connection successful.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);

}
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                    QObject::tr("connection failed.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);


    //mailer::sendEmail();
    return a.exec();
}

#include "menu.h"
#include "ui_menu.h"

#include "mainwindow_fac.h"
#include "mainwindow_f.h"
#include "QMainWindow"



menu::menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
}

menu::~menu()
{
    delete ui;
}

void menu::on_pb_fact_clicked()
{
    MainWindow_fac * mainwindow_fac =new MainWindow_fac;
    mainwindow_fac->show();
}

void menu::on_pb_fr_clicked()
{
  MainWindow_f * mainwindowf =new MainWindow_f;
   mainwindowf ->show();
}

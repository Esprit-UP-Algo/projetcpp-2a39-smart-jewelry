#ifndef MAINWINDOW_F_H
#define MAINWINDOW_F_H
#include "facture.h"
#include <QMainWindow>
#include <QQmlContext>
#include "arduino.h"

namespace Ui {
class MainWindow_f;
}

class MainWindow_f : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_f(QWidget *parent = nullptr);
    ~MainWindow_f();

private slots:
    void on_pb_ajouter_clicked();

    void on_pb_supprimer_clicked();

    void on_pb_modifier_clicked();

    void on_pb_notfier_clicked();

    void on_pb_enregistrer_clicked();

    void on_p_pdf_clicked();

    void on_pb_idf_clicked();

    void on_pb_typep_clicked();

    void on_pb_nom_clicked();

    void on_pb_nom_2_clicked();

    void on_pb_num_tel_clicked();

    void on_pb_idf_2_clicked();

    void on_displayCharts_clicked();

    void on_Localiser_clicked();


signals:
    void setCenter(QVariant, QVariant);
    void addMarker(QVariant, QVariant);
    void addMarker2(QVariant, QVariant);
    void addMarker3(QVariant, QVariant);
    void addMarker4(QVariant, QVariant);
private:
    Ui::MainWindow_f*ui;
    Facture f;
     QQmlContext* m_qmlContext;
     QString nomFacture;
     QString prenomFacture;
     Arduino ard;


};

#endif // MAINWINDOW_H

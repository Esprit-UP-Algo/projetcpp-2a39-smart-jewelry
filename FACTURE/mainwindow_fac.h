#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "fournisseur.h"
#include <QMainWindow>

namespace Ui {
class MainWindow_fac;
}

class MainWindow_fac : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_fac(QWidget *parent = nullptr);
    ~MainWindow_fac();

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

private:

    Ui::MainWindow_fac *ui;
    Fournisseur f;
};

#endif // MAINWINDOW_H

#ifndef MENU_H
#define MENU_H

#include <QMainWindow>

namespace Ui {
class menu;
}

class menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = nullptr);
    ~menu();

private slots:
    void on_pb_fact_clicked();

    void on_pb_fr_clicked();

private:
    Ui::menu *ui;
};

#endif // MENU_H

#ifndef FACTURE_H
#define FACTURE_H

#include <QString>
#include <QSqlQueryModel>

class Facture
{
public:
    Facture();
    Facture(int, QString, QString, QString, int, QString, QString);
    int getidf();
    QString getnom();
    QString getprenom();
    QString getemail();
    QString gettypep();
    QString getadresse();
    int getnum_tel();
    void setidf(int);
    void setnom(QString);
    void setprenom(QString);
    void setemail(QString);
    void settype(QString);
    void setadresse(QString);
    void setnum_tel(int);
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool existe(int);
    bool modifier(int idf, QString nNom, QString nPrenom, QString nEmail, int nnum_tel, QString typep, QString adresse);
    QSqlQueryModel* trier(QString triPar);
    QSqlQueryModel* rechercherFacture(const QString critere);

private:
    int idf;
    int num_tel;
    QString nom, prenom, email, typep, adresse;
};

#endif // FACTURE_H

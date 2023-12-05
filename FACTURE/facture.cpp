#include "facture.h"
#include <QSqlQuery>
#include <QtDebug>
#include "mainwindow_f.h"
#include <QMessageBox>
#include <QObject>

Facture::Facture()
{
idf=0 ;
nom=" " ;
prenom=" " ;
email=" " ;
num_tel=0 ;
typep="";
adresse="";
}

Facture::Facture(int IDF, QString NOM, QString PRENOM, QString EMAIL,int NUM_TEL, QString TYPEP, QString ADRESSE)
{
 this->idf=IDF ;
 this->nom=NOM ;
 this->prenom=PRENOM ;
 this->email=EMAIL ;
 this->num_tel=NUM_TEL ;
 this->typep=TYPEP;
 this->adresse=ADRESSE;
}
int Facture::getidf()
{
    return idf;}
QString Facture::getnom()
{
    return nom;}
QString Facture::getprenom()
{
    return prenom;}
QString Facture::getemail()
{
    return email;}
QString Facture::gettypep()
{
    return typep;}
QString Facture::getadresse()
{
    return adresse;}
int Facture::getnum_tel()
{
    return num_tel;}
void Facture::setidf(int IDF)
{this->idf=IDF;}
void Facture::setnom(QString NOM)
{this->nom=NOM ;}
void Facture::setprenom(QString PRENOM)
{this->prenom=PRENOM ;}
void Facture::setemail(QString EMAIL)
{this->email=EMAIL ;}
void Facture::settype(QString TYPEP)
{this->typep=TYPEP ;}
void Facture::setadresse(QString ADRESSE)
{this->adresse=ADRESSE;}
void Facture::setnum_tel(int NUM_TEL)
{this->num_tel=NUM_TEL;}

bool Facture::ajouter()
{
    bool test = false;

    QSqlQuery query;
    QString idf_string = QString::number(idf);
    query.prepare("INSERT INTO FACTURE (IDF,  NOM, PRENOM, EMAIL, NUM_TEL, TYPEP, ADRESSE) "
                  "VALUES (:idf, :nom, :prenom, :email, :num_tel, :typep, :adresse)");
    query.bindValue(":idf", idf_string);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":num_tel", num_tel);
    query.bindValue(":typep", typep);
    query.bindValue(":adresse", adresse);
    test = query.exec();

    return test;
}

QSqlQueryModel* Facture::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM FACTURE");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numero de telephone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type de produit"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));

    return model;
}

bool Facture::supprimer(int idf)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FACTURE WHERE IDF = :idf");
    query.bindValue(":idf", idf);
    return query.exec();
}

bool Facture::existe(int idf)
{
    QSqlQuery query;
    QString idf_string = QString::number(idf);
    query.prepare("SELECT COUNT(*) FROM FACTURE WHERE IDF = :idf");
    query.bindValue(":idf", idf_string);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }

    return false;
}

bool Facture::modifier(int idf, QString nNom, QString nPrenom, QString nEmail, int nnum_tel, QString ntypep, QString nadresse)
{
    QSqlQuery query;
    QString idf_string = QString::number(idf);
    query.prepare("UPDATE FACTURE SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, NUM_TEL = :numTel, TYPEP = :typep, ADRESSE = :adresse WHERE IDF = :idf");
    query.bindValue(":nom", nNom);
    query.bindValue(":prenom", nPrenom);
    query.bindValue(":email", nEmail);
    query.bindValue(":numTel", nnum_tel);
    query.bindValue(":idf", idf_string);
    query.bindValue(":typep", ntypep);
    query.bindValue(":adresse", nadresse);
    return query.exec();
}

QSqlQueryModel* Facture::trier(QString triPar)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString query = "SELECT * FROM FACTURE ORDER BY ";
    if (triPar == "idf")
    {
        query += "IDF";
    }
    else if (triPar == "typep")
    {
        query += "TYPEP";
    }
    else if (triPar == "nom")
    {
        query += "NOM";
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numero de telephone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type de produit"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));

    return model;
}

QSqlQueryModel* Facture::rechercherFacture(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString query = "SELECT * FROM FACTURE WHERE IDF LIKE '%" + critere + "%' OR NUM_TEL LIKE '%" + critere + "%' OR NOM LIKE '%" + critere +"%' OR TYPEP LIKE '%" + critere + "%'";
            model->setQuery(query);
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numero de telephone"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type de produit"));
            model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));

            return model;
        }

#include "fournisseur.h"
#include <QSqlQuery>
#include <QtDebug>
#include "mainwindow_fac.h"
#include <QMessageBox>
#include <QObject>

Fournisseur::Fournisseur()
{
idf=0 ;
nom=" " ;
prenom=" " ;
email=" " ;
num_tel=0 ;
typep="";
adresse="";
}

Fournisseur::Fournisseur(int IDF, QString NOM, QString PRENOM, QString EMAIL,int NUM_TEL, QString TYPEP, QString ADRESSE)
{
 this->idf=IDF ;
 this->nom=NOM ;
 this->prenom=PRENOM ;
 this->email=EMAIL ;
 this->num_tel=NUM_TEL ;
 this->typep=TYPEP;
 this->adresse=ADRESSE;
}
int Fournisseur::getidf()
{
    return idf;}
QString Fournisseur::getnom()
{
    return nom;}
QString Fournisseur::getprenom()
{
    return prenom;}
QString Fournisseur::getemail()
{
    return email;}
QString Fournisseur::gettypep()
{
    return typep;}
QString Fournisseur::getadresse()
{
    return adresse;}
int Fournisseur::getnum_tel()
{
    return num_tel;}
void Fournisseur::setidf(int IDF)
{this->idf=IDF;}
void Fournisseur::setnom(QString NOM)
{this->nom=NOM ;}
void Fournisseur::setprenom(QString PRENOM)
{this->prenom=PRENOM ;}
void Fournisseur::setemail(QString EMAIL)
{this->email=EMAIL ;}
void Fournisseur::settype(QString TYPEP)
{this->typep=TYPEP ;}
void Fournisseur::setadresse(QString ADRESSE)
{this->adresse=ADRESSE;}
void Fournisseur::setnum_tel(int NUM_TEL)
{this->num_tel=NUM_TEL;}

bool Fournisseur::ajouter()
{

        bool test = false;

        QSqlQuery query;
        QString idf_string = QString::number(idf);
        query.prepare("INSERT INTO FOURNISSEUR (IDF,  NOM, PRENOM, EMAIL, NUM_TEL, TYPEP, ADRESSE) "
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

QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();

          model->setQuery("SELECT* FROM FOURNISSEUR");
          model->setHeaderData(0, Qt::Horizontal, QObject::tr("Identifiant"));
          model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
          model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
          model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
          model->setHeaderData(4, Qt::Horizontal, QObject::tr("Numero de telephone"));
          model->setHeaderData(5, Qt::Horizontal, QObject::tr("Type de produit"));
          model->setHeaderData(6, Qt::Horizontal, QObject::tr("Adresse"));
    return model;


}
bool Fournisseur::supprimer(int idf)
{

            QSqlQuery query;
            query.prepare(" Delete from FOURNISSEUR where idf=:id");
            query.bindValue(0,idf);
            return query.exec();

}

bool Fournisseur::existe(int idf)
{
    QSqlQuery query;
    QString idf_string = QString::number(idf);
    query.prepare("SELECT COUNT(*) FROM FOURNISSEUR WHERE IDF = :idf");
    query.bindValue(":idf", idf_string);

    if (query.exec() && query.next()) {
        int count=query.value(0).toInt();
        return count>0;
    }

    return false;
}
bool Fournisseur::modifier(int idf, QString nNom, QString nPrenom, QString nEmail, int nnum_tel, QString ntypep, QString nadresse)
{
    QSqlQuery query;
    QString idf_string = QString::number(idf);
    query.prepare("UPDATE FOURNISSEUR SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, NUM_TEL = :numTel, TYPEP = :typep, ADRESSE = :adresse WHERE IDF = :idf");
    query.bindValue(":nom", nNom);
    query.bindValue(":prenom", nPrenom);
    query.bindValue(":email", nEmail);
    query.bindValue(":numTel", nnum_tel);
    query.bindValue(":idf", idf_string);
    query.bindValue(":typep", ntypep);
    query.bindValue(":adresse", nadresse);
    return query.exec();
}
QSqlQueryModel* Fournisseur::trier(QString triPar)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString query = "SELECT * FROM FOURNISSEUR ORDER BY ";
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



QSqlQueryModel* Fournisseur::rechercherFournisseur(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString query = "SELECT * FROM FOURNISSEUR WHERE IDF LIKE '%" + critere + "%' OR NUM_TEL LIKE '%" + critere + "%' OR NOM LIKE '%" + critere + "%'";
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

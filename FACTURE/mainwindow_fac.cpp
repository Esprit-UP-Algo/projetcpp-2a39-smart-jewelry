#include "mainwindow_fac.h"
#include "ui_mainwindow_fac.h"
#include "fournisseur.h"
#include <QMessageBox>
#include <QIntValidator>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtDebug>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QTextOption>
#include <QFont>
#include <QMap>
#include <QString>
#include <QSqlQuery>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QtMath>



MainWindow_fac::MainWindow_fac(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_fac)
{
    ui->setupUi(this);
    ui->le_idf->setValidator(new QIntValidator(1, 9999, this));
    QStringList regexList;
    regexList << "2[0-9]{7}" << "5[0-9]{7}" << "9[0-9]{7}" << "7[0-9]{7}";
    QString combinedRegex = "^(" + regexList.join("|") + ")$";
    QRegExp regex(combinedRegex);
    QRegExpValidator* validator = new QRegExpValidator(regex, this);
    ui->le_num_tel->setValidator(validator);
    ui->tab_fournisseur->setModel(f.afficher());
    ui->le_num_tel_2->setValidator(validator);
    ui->le_nom_2->setEnabled(false);
    ui->le_prenom_2->setEnabled(false);
    ui->le_email_2->setEnabled(false);
    ui->le_num_tel_2->setEnabled(false);
    ui->le_typep_2->setEnabled(false);
    ui->le_adresse_2->setEnabled(false);
}

MainWindow_fac::~MainWindow_fac()
{
    delete ui;
}

void MainWindow_fac::on_pb_ajouter_clicked()
{
    int IDF=ui->le_idf->text().toInt();
    QString NOM=ui->le_nom->text();
    QString PRENOM=ui->le_prenom->text();
    QString EMAIL=ui->le_email->text();
    int NUM_TEL=ui->le_num_tel->text().toInt();
    QString TYPEP=ui->le_typep->text();
    QString ADRESSE=ui->le_adresse->text();


       // Contrôle de saisie pour le champ "Type de produit"
       QStringList allowedTypes = {"or", "cuivre", "argent", "autre", "pierre"};
       if (!allowedTypes.contains(TYPEP, Qt::CaseInsensitive)) {
           QMessageBox::warning(this, "Erreur", "Le type de produit n'est pas valide.");
           return; // Arrêtez le traitement si le type de produit est invalide.
        }

    // Contrôle de saisie pour l'adresse e-mail
            QRegExp emailPattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
            if (!emailPattern.exactMatch(EMAIL)) {
                QMessageBox::warning(this, "Erreur", "L'adresse e-mail n'est pas valide.");
                return; // Arrêtez le traitement si l'adresse e-mail est invalide.


            }
    bool test1 =f.existe(IDF);
        if(test1)
            {
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                        QObject::tr("Ajout failed :Identifant existe deja \n"
                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);


            QSqlQueryModel* model = f.afficher();
            ui->tab_fournisseur->setModel(model);

    }
        else
        {
            //refresh liste des fournisseurs apres chaque ajout
            Fournisseur f(IDF, NOM, PRENOM, EMAIL, NUM_TEL,TYPEP,ADRESSE);
            bool test1= f.ajouter();
         if(test1)
         {
            QMessageBox::information(nullptr,QObject::tr("OK"),
                        QObject::tr("ajout effectué.\n"
                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);

            QSqlQueryModel* model = f.afficher();
            ui->tab_fournisseur->setModel(model);
            ui->le_idf->clear();
            ui->le_nom->clear();
            ui->le_prenom->clear();
            ui->le_email->clear();
            ui->le_num_tel->clear();
            ui->le_typep->clear();
            ui->le_adresse->clear();
        }
        else//si l'ajout a echoué
        {
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                        QObject::tr("ajout failed. \n"
                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            QSqlQueryModel* model = f.afficher();
            ui->tab_fournisseur->setModel(model);
         }
        }

}



void MainWindow_fac::on_pb_supprimer_clicked()
{
    Fournisseur f;
    f.setidf(ui->le_id_supp->text().toInt());
    bool test=f.existe(f.getidf());
    QMessageBox msgBox;

    if(test)
    {
        test=f.supprimer(f.getidf());
        if(test)
    {
            QMessageBox::information(nullptr,QObject::tr("OK"),
                        QObject::tr("Supression effectué.\n"
                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            ui->tab_fournisseur->setModel(f.afficher());
            ui->le_id_supp->clear();
    }
    else
        {
            QMessageBox::warning(nullptr, QObject::tr("NOT OK"),
                                        QObject::tr("supression failed.\n"
                                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }}
    else
        {
            QMessageBox::warning(nullptr, QObject::tr("NOT OK"),
                                        QObject::tr("supression failed. identifiant n'existe pas \n"
                                                    "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        }

}

void MainWindow_fac::on_pb_modifier_clicked()
{
    int IDF=ui->le_idf_2->text().toInt();
    bool test1 =f.existe(IDF);

    if (test1)
           { {
        QMessageBox::information(nullptr,QObject::tr("OK"),
                         QObject::tr("Ce Fournisseur existe.\n"
                                     "Click Cancel to exit."), QMessageBox::Cancel);

        QSqlQuery query;
        query.prepare("SELECT NOM, PRENOM, EMAIL, NUM_TEL, TYPEP, ADRESSE FROM FOURNISSEUR WHERE IDF = :idf");
        query.bindValue(":idf", IDF);

                    if (query.exec() && query.next())
        {

                 QString nNom = query.value(0).toString();
                 QString nPrenom = query.value(1).toString();
                 QString nEmail = query.value(2).toString();
                 int nnum_tel = query.value(3).toInt();
                 QString ntypep = query.value(4).toString();
                 QString nadresse = query.value(5).toString();

                     ui->le_idf_2->setText(QString::number(IDF));
                     ui->le_nom_2->setText(nNom);
                     ui->le_prenom_2->setText(nPrenom);
                     ui->le_email_2->setText(nEmail);
                     ui->le_num_tel_2->setText(QString::number(nnum_tel));
                     ui->le_typep_2->setText(ntypep);
                     ui->le_adresse_2->setText(nadresse);

                     ui->le_nom_2->setEnabled(true);
                     ui->le_prenom_2->setEnabled(true);
                     ui->le_email_2->setEnabled(true);
                     ui->le_num_tel_2->setEnabled(true);
                     ui->le_typep_2->setEnabled(true);
                     ui->le_adresse_2->setEnabled(true);

                    }

                    QSqlQueryModel* model = f.afficher();
                    ui->tab_fournisseur->setModel(model);

                 //ui->tab_fournisseur->setModel(f.afficher());


            }



    }
     else

     {                      QMessageBox::warning(nullptr, QObject::tr("NOT OK"),
                            QObject::tr("Identifiant inexsistant .\n"
                                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            QSqlQueryModel* model = f.afficher();
            ui->tab_fournisseur->setModel(model);
    }

}


void MainWindow_fac::on_pb_notfier_clicked()
{


}

void MainWindow_fac::on_pb_enregistrer_clicked()
{   QStringList regexList;
    regexList << "2[0-9]{7}" << "5[0-9]{7}" << "9[0-9]{7}" << "7[0-9]{7}";
    QString combinedRegex = "^(" + regexList.join("|") + ")$";
    QRegExp regex(combinedRegex);
    int IDF = ui->le_idf_2->text().toInt();
        QString nom = ui->le_nom_2->text();
        QString prenom = ui->le_prenom_2->text();
        QString email = ui->le_email_2->text();
        int num_tel = ui->le_num_tel_2->text().toInt();
        QString typep = ui->le_typep_2->text();
        QString adresse = ui->le_adresse_2->text();

        QRegExp emailPattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
        if (!emailPattern.exactMatch(email))
        {
            QMessageBox::warning(this, "Erreur", "L'adresse e-mail n'est pas valide.");
            return; // Arrêtez le traitement si l'adresse e-mail est invalide.
        }
        // Contrôle de saisie pour le champ "Type de produit"
          QStringList allowedTypes = {"or", "cuivre", "argent", "autre ", "pierre"};
          if (!allowedTypes.contains(typep, Qt::CaseInsensitive))
          {
              QMessageBox::warning(this, "Erreur", "Le type de produit n'est pas valide.");
              return; // Arrêtez le traitement si le type de produit est invalide.
          }
        // Appeler une fonction de mise à jour dans votre classe Fournisseur
        f.modifier(IDF, nom, prenom, email, num_tel, typep, adresse);

        // Désactiver les champs après la mise à jour
        ui->le_nom_2->setEnabled(false);
        ui->le_prenom_2->setEnabled(false);
        ui->le_email_2->setEnabled(false);
        ui->le_num_tel_2->setEnabled(false);
        ui->le_typep_2->setEnabled(false);
        ui->le_adresse_2->setEnabled(false);
        QMessageBox::information(nullptr,QObject::tr("OK"),
                    QObject::tr("Modiffication effectué.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->le_idf_2->clear();
        ui->le_nom_2->clear();
        ui->le_prenom_2->clear();
        ui->le_email_2->clear();
        ui->le_num_tel_2->clear();
        ui->le_typep_2->clear();
        ui->le_adresse_2->clear();
        QSqlQueryModel* model = f.afficher();
        ui->tab_fournisseur->setModel(model);

}

void MainWindow_fac::on_p_pdf_clicked()
{
    // Sélectionner l'emplacement de sauvegarde du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty()) {
        // Créer un objet QPdfWriter avec le chemin du fichier PDF
        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSizeMM(QSizeF(297, 210)); //

        // Ouvrir le document PDF en écriture
        QPainter painter(&pdfWriter);
        if (!painter.isActive()) {
            return;
        }
        // Récupérer les données du fournisseur
        Fournisseur fournisseur;
        QSqlQueryModel* model = fournisseur.afficher();
        if (!model) {
            return;
        }

        // Définir les marges de la page
        const qreal leftMargin = 20;
        const qreal topMargin = 20;
        const qreal rightMargin = 20;
        const qreal bottomMargin = 20;

        pdfWriter.setPageMargins(QMarginsF(leftMargin, topMargin, rightMargin, bottomMargin));

        // Récupérer les marges de la page
        const QMarginsF pageMargins = pdfWriter.pageLayout().margins();
        const qreal pageLeftMargin = pageMargins.left();
        const qreal pageTopMargin = pageMargins.top();
        const qreal pageRightMargin = pageMargins.right();
        const qreal pageBottomMargin = pageMargins.bottom();

        // Calculer la largeur et la hauteur disponibles pour le tableau
        const qreal tableWidth = pdfWriter.width() - pageLeftMargin - pageRightMargin;
        const qreal tableHeight = pdfWriter.height() - pageTopMargin - pageBottomMargin;

        // Définir les propriétés du tableau
        const int rowCount = model->rowCount();
        const int columnCount = model->columnCount();
        const qreal lineHeight = 300; // Hauteur de ligne réduite pour diminuer l'espace vertical
        const qreal columnWidth = 2000; // Largeur de colonne égale pour chaque élément
        const QFont font("Arial", 10);

        // Dessiner le tableau
        qreal x = pageLeftMargin;
        qreal y = pageTopMargin;

        // Dessiner les en-têtes de colonne
        for (int column = 0; column < columnCount; ++column) {
            QRectF cellRect(x, y, columnWidth, lineHeight);
            painter.setFont(font);
            painter.drawText(cellRect, Qt::AlignCenter, model->headerData(column, Qt::Horizontal).toString());
            x += columnWidth;
        }

        y += lineHeight;

        // Dessiner les données du fournisseur
        for (int row = 0; row < rowCount; ++row) {
            x = pageLeftMargin;

            for (int column = 0; column < columnCount; ++column) {
                QRectF cellRect(x, y, columnWidth, lineHeight);
                painter.setFont(font);
                painter.drawText(cellRect, Qt::AlignLeft | Qt::AlignVCenter, model->data(model->index(row, column)).toString());
                x += columnWidth;
            }

            y += lineHeight;

            // Dessiner un trait horizontal
            painter.setPen(QPen(Qt::black, 1));
            painter.drawLine(pageLeftMargin, y, pageLeftMargin + tableWidth, y);
        }

        // Fermer le document PDF
        painter.end();

        QMessageBox::information(this, "PDF généré", "Le fichier PDF a été généré avec succès.");
    }
}

void MainWindow_fac::on_pb_idf_clicked()
{
    QString triPar = "idf";
       QSqlQueryModel* model = f.trier(triPar);
       ui->tab_fournisseur_2->setModel(model);
}

void MainWindow_fac::on_pb_typep_clicked()
{
    QString triPar = "typep";
       QSqlQueryModel* model = f.trier(triPar);
       ui->tab_fournisseur_2->setModel(model);
}

void MainWindow_fac::on_pb_nom_clicked()
{
    QString triPar = "nom";
       QSqlQueryModel* model = f.trier(triPar);
       ui->tab_fournisseur_2->setModel(model);
}

void MainWindow_fac::on_pb_nom_2_clicked()
{
    QString critere = ui->lineEdit_critere->text();
    QSqlQueryModel* model = f.rechercherFournisseur(critere);
    ui->tab_fournisseur_3->setModel(model);
}

void MainWindow_fac::on_pb_num_tel_clicked()
{
    QString critere = ui->lineEdit_critere->text();
    QSqlQueryModel* model = f.rechercherFournisseur(critere);
    ui->tab_fournisseur_3->setModel(model);
}

void MainWindow_fac::on_pb_idf_2_clicked()
{
    QString critere = ui->lineEdit_critere->text();
    QSqlQueryModel* model = f.rechercherFournisseur(critere);
    ui->tab_fournisseur_3->setModel(model);
}




void MainWindow_fac::on_displayCharts_clicked()
{
    QMap<QString, int> typeCount;  // Map to store the count of each type

    // Retrieve data from the database and populate typeCount
    QSqlQuery query("SELECT TYPEP, COUNT(*) FROM FOURNISSEUR GROUP BY TYPEP");
    while (query.next()) {
        QString type = query.value(0).toString();
        int count = query.value(1).toInt();
        typeCount[type] = count;
    }

    // Calculate total count
    int totalCount = 0;
    for (int count : typeCount.values()) {
        totalCount += count;
    }

    // Create a QGraphicsScene to display the charts
    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    // Draw pie chart based on typeCount data and add percentages as labels
    qreal startAngle = 0;
    for (const QString& type : typeCount.keys()) {
        qreal angle = 360.0 * typeCount[type] / totalCount;
        QColor sliceColor = Qt::red; // Adjust colors as desired

        QGraphicsEllipseItem* slice = scene->addEllipse(-50, -50, 100, 100, QPen(Qt::black), QBrush(sliceColor));
        slice->setStartAngle(startAngle * 16);
        slice->setSpanAngle(angle * 16);

        // Calculate and display percentage as label
        qreal percentage = 100.0 * typeCount[type] / totalCount;
        QString percentageText = QString::number(percentage, 'f', 1) + "%";
        QGraphicsTextItem* label = scene->addText(percentageText, QFont("Lucida Bright", 8));
        label->setPos(70 * qCos(qDegreesToRadians(startAngle + angle / 2)),
                      70 * qSin(qDegreesToRadians(startAngle + angle / 2)));

        startAngle += angle;
    }

    // Add legends for each type with updated colors
    int legendX = 180;
    int legendY = 20;
    int legendSpacing = 25;

    for (const QString& type : typeCount.keys()) {
        scene->addRect(legendX, legendY, 20, 20, QPen(Qt::black), QBrush(Qt::blue)); // Adjust colors as desired
        scene->addText(type, QFont("Lucida Bright", 10))->setPos(legendX + 30, legendY + 5);

        legendY += legendSpacing;
    }

    // Set up QGraphicsView properties
    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->show();
}

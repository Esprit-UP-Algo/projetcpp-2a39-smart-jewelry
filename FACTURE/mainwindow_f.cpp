#include "mainwindow_f.h"
#include "ui_mainwindow_f.h"
#include "facture.h"
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
#include <QString>
#include <QSqlQuery>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QtMath>
#include <QQuickItem>
#include <QQuickView>
#include <QQmlEngine>
#include "mailer.h"
#include <QtNetwork>
#include <QDebug>
#include "Sms.h"
#include "arduino.h"
MainWindow_f::MainWindow_f(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow_f)
{
    ui->setupUi(this);
    ui->le_idf->setValidator(new QIntValidator(1, 9999, this));
    QStringList regexList;
    regexList << "2[0-9]{7}" << "5[0-9]{7}" << "9[0-9]{7}" << "7[0-9]{7}";
    QString combinedRegex = "^(" + regexList.join("|") + ")$";
    QRegExp regex(combinedRegex);
    QRegExpValidator* validator = new QRegExpValidator(regex, this);
    ui->le_num_tel->setValidator(validator);
    ui->tab_Facture->setModel(f.afficher());
    ui->le_num_tel_2->setValidator(validator);

    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    auto obj = ui->quickWidget->rootObject();
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));

    emit setCenter(36.898237, 10.189049);
    emit addMarker(36.898237, 10.189049);


    ui->le_nom_2->setEnabled(false);
    ui->le_prenom_2->setEnabled(false);
    ui->le_email_2->setEnabled(false);
    ui->le_num_tel_2->setEnabled(false);
    ui->le_typep_2->setEnabled(false);
    ui->le_adresse_2->setEnabled(false);
    int ret= ard.connect_arduino();
            {// Attempt to connect to Arduino

    switch (ret) {
        case 0:
            qDebug() << "Arduino connected successfully!";
            // Proceed with other operations, e.g., data retrieval
            break;
        case 1:
            qDebug() << "Failed to find Arduino port!";
            // Handle failure to find Arduino port
            break;
        case 2:
            qDebug() << "Arduino is already connected!";
            // Handle case where Arduino is already connected
            break;
        default:
            qDebug() << "Unknown error occurred!";
            // Handle any other unexpected errors
            break;
    }}




}

MainWindow_f::~MainWindow_f()
{
    delete ui;
}

void MainWindow_f::on_pb_ajouter_clicked()
{
    int idf = ui->le_idf->text().toInt();
    QString nom = ui->le_nom->text();
    QString prenom = ui->le_prenom->text();
    QString email = ui->le_email->text();
    int num_tel = ui->le_num_tel->text().toInt();
    QString typeProduit = ui->le_typep->text();
    QString adresse = ui->le_adresse->text();

    // Contrôle de saisie pour le champ "Type de produit"
    QStringList allowedTypes = {"or", "cuivre", "argent", "autre", "pierre"};
    if (!allowedTypes.contains(typeProduit, Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "Erreur", "Le type de produit n'est pas valide.");
        return; // Arrêtez le traitement si le type de produit est invalide.
    }

    // Contrôle de saisie pour l'adresse e-mail
    QRegExp emailPattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
    if (!emailPattern.exactMatch(email)) {
        QMessageBox::warning(this, "Erreur", "L'adresse e-mail n'est pas valide.");
        return; // Arrêtez le traitement si l'adresse e-mail est invalide.
    }

    bool existe = f.existe(idf);
    if (existe) {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Ajout failed : Identifant existe deja \n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);

        QSqlQueryModel* model = f.afficher();
        ui->tab_Facture->setModel(model);
    } else {

        Facture nouvelleFacture(idf, nom, prenom, email, num_tel
                               , typeProduit, adresse);
        bool ajoutReussi = nouvelleFacture.ajouter();
        if (ajoutReussi) {
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué.\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);

            QSqlQueryModel* model = f.afficher();
            ui->tab_Facture->setModel(model);

            // Effacer les champs de saisie
            ui->le_idf->clear();
            ui->le_nom->clear();
            ui->le_prenom->clear();
            ui->le_email->clear();
            ui->le_num_tel->clear();
            ui->le_typep->clear();
            ui->le_adresse->clear();
        } else {
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                                  QObject::tr("Ajout failed. \n"
                                              "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);

            QSqlQueryModel* model = f.afficher();
            ui->tab_Facture->setModel(model);
        }
    }
}

void MainWindow_f::on_pb_supprimer_clicked()
{
    Facture f;
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
            ui->tab_Facture->setModel(f.afficher());
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

void MainWindow_f::on_pb_modifier_clicked()
{
    int IDF=ui->le_idf_2->text().toInt();
    bool test1 =f.existe(IDF);

    if (test1)
           { {
        QMessageBox::information(nullptr,QObject::tr("OK"),
                         QObject::tr("Cette Facture existe.\n"
                                     "Click Cancel to exit."), QMessageBox::Cancel);

        QSqlQuery query;
        query.prepare("SELECT NOM, PRENOM, EMAIL, NUM_TEL, TYPEP, ADRESSE FROM FACTURE WHERE IDF = :idf");
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
                    ui->tab_Facture->setModel(model);




            }



    }
     else

     {                      QMessageBox::warning(nullptr, QObject::tr("NOT OK"),
                            QObject::tr("Identifiant inexsistant .\n"
                                        "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
            QSqlQueryModel* model = f.afficher();
            ui->tab_Facture->setModel(model);
    }

}

void MainWindow_f::on_pb_notfier_clicked()
{

    Facture f;
    int id=ui->le_idf_3->text().toInt();

        sms s;
       // QString recipientEmail = emailSender.fetchRecipientEmailFromDatabase(id);



        QDateTime dateTime = ui->dateTimeEdit->dateTime();  // Récupérer la date et l'heure de livraison depuis le dateTimeEdit

            QString dateLivraison = dateTime.toString("yyyy-MM-dd");
            QString heureLivraison = dateTime.toString("HH:mm:ss");
            QString nomFacture;
            QString prenomFacture;
        QSqlQuery query;
        QString id_string = QString::number(id);
        //query.prepare("SELECT COUNT(*) FROM FACTURE WHERE IDF = :id");
        //query.bindValue(":idf", id_string);
        query.prepare("SELECT NOM, PRENOM FROM FACTURE WHERE IDF = :id");
        query.bindValue(":id", id_string);
        if (query.exec() && query.next()) {
                    nomFacture = query.value(0).toString();
                    prenomFacture = query.value(1).toString();
                    this->nomFacture = nomFacture;
                    this->prenomFacture = prenomFacture;
                }





                    s.sendSMS("+21624815019", "REMINDER    \n Cher " + nomFacture + " " + prenomFacture + ",\n\n Nous vous informons que la livraison est prévue le " + dateLivraison + " à " + heureLivraison + ".\ Merci pour votre collaboration.\n\n Cordialement,\n JewelPro")

;}
void MainWindow_f::on_pb_enregistrer_clicked()
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

        f.modifier(IDF, nom, prenom, email, num_tel, typep, adresse);


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
        ui->tab_Facture->setModel(model);

}

void MainWindow_f::on_p_pdf_clicked()
{
    // Sélectionner l'emplacement de sauvegarde du fichier PDF
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty()) {
        // Créer un objet QPdfWriter avec le chemin du fichier PDF
        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSizeMM(QSizeF(297, 210)); // A4 (210mm x 297mm)

        // Ouvrir le document PDF en écriture
        QPainter painter(&pdfWriter);
        if (!painter.isActive()) {
            return;
        }

        // Récupérer les données du Facture
        Facture f;
        QSqlQueryModel* model = f.afficher();
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

        // Dessiner les données du Facture
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

void MainWindow_f::on_pb_idf_clicked()
{
    QString triPar = "idf";
       QSqlQueryModel* model = f.trier(triPar);
       ui->tab_Facture_2->setModel(model);
}

void MainWindow_f::on_pb_typep_clicked()
{
    QString triPar = "typep";
       QSqlQueryModel* model = f.trier(triPar);
       ui->tab_Facture_2->setModel(model);
}

void MainWindow_f::on_pb_nom_clicked()
{
    QString triPar = "nom";
       QSqlQueryModel* model = f.trier(triPar);
       ui->tab_Facture_2->setModel(model);
}

void MainWindow_f::on_pb_nom_2_clicked()
{
    QString critere = ui->lineEdit_critere->text();
    QSqlQueryModel* model = f.rechercherFacture(critere);
    ui->tab_Facture_3->setModel(model);
}

void MainWindow_f::on_pb_num_tel_clicked()
{
    QString critere = ui->lineEdit_critere->text();
    QSqlQueryModel* model = f.rechercherFacture(critere);
    ui->tab_Facture_3->setModel(model);
}

void MainWindow_f::on_pb_idf_2_clicked()
{
    QString critere = ui->lineEdit_critere->text();
    QSqlQueryModel* model = f.rechercherFacture(critere);
    ui->tab_Facture_3->setModel(model);
}

void MainWindow_f::on_displayCharts_clicked()
{
    QMap<QString, int> typeCount;


    QSqlQuery query("SELECT TYPEP, COUNT(*) FROM FACTURE GROUP BY TYPEP");
    while (query.next()) {
        QString typep = query.value(0).toString();
        int count = query.value(1).toInt();
        typeCount[typep] = count;
    }


    int totalCount = 0;
    for (int count : typeCount.values()) {
        totalCount += count;
    }

    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsView* view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);


    QList<QColor> sliceColors = {
        Qt::red,
        Qt::green,
        Qt::blue,
        Qt::yellow,
        Qt::magenta,
        Qt::cyan,
        Qt::gray,

    };


    qreal startAngle = 0;
    int colorIndex = 0;
    for (const QString& typep : typeCount.keys()) {
        qreal angle = 360.0 * typeCount[typep] / totalCount;
        QColor sliceColor = sliceColors[colorIndex % sliceColors.size()];
        QGraphicsEllipseItem* slice = scene->addEllipse(-50, -50, 100, 100, QPen(Qt::black), QBrush(sliceColor));
        slice->setStartAngle(startAngle * 16);
        slice->setSpanAngle(angle * 16);

        qreal percentage = 100.0 * typeCount[typep] / totalCount;
        QString percentageText = QString::number(percentage, 'f', 1) + "%";
        QGraphicsTextItem* label = scene->addText(percentageText, QFont("Lucida Bright", 10));
            label->setDefaultTextColor(Qt::black); // Set text color to black for better visibility
            label->setPos(70 * qCos(qDegreesToRadians(startAngle + angle / 2)) - label->boundingRect().width() / 2,
                          70 * qSin(qDegreesToRadians(startAngle + angle / 2)) - label->boundingRect().height() / 2);

        startAngle += angle;
        colorIndex++;
    }


    int legendX = 180;
    int legendY = 200;
    int legendSpacing = 25;

    colorIndex = 0;
    for (const QString& type : typeCount.keys()) {
        QColor sliceColor = sliceColors[colorIndex % sliceColors.size()];

        scene->addRect(legendX, legendY, 20, 20, QPen(Qt::black), QBrush(sliceColor));
        scene->addText(type, QFont("Lucida Bright", 10))->setPos(legendX + 30, legendY + 5);

        legendY += legendSpacing;
        colorIndex++;
    }


    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->show();
}



void MainWindow_f::on_Localiser_clicked()
{

    QString critere = ui->lineEdit_critere_2->text();
    QSqlQueryModel* model = f.rechercherFacture(critere);
    ui->tab_Facture_4->setModel(model);

    auto obj = ui->quickWidget->rootObject();
    connect(this, SIGNAL(addMarker2(QVariant, QVariant)), obj, SLOT(addMarker2(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker3(QVariant, QVariant)), obj, SLOT(addMarker3(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker4(QVariant, QVariant)), obj, SLOT(addMarker4(QVariant, QVariant)));
    emit addMarker2(37.272017, 9.874719);
    emit addMarker3(36.878702, 10.172032);
    emit addMarker4(36.867705, 10.338056);
    emit addMarker4(36.645837, 10.591638);
    emit addMarker4(36.927982, 10.1513);
    emit addMarker4(37.098446, 9.836673);
}



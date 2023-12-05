#ifndef MAILER_H
#define MAILER_H
#include <QString>

class mailer
{
public:
    mailer();
    int sendEmail(const QString& recipientEmail,const QString& body, const QString& nomFournisseur, const QString& prenomFournisseur);
    QString fetchRecipientEmailFromDatabase(int id);

};

#endif // MAILER_H

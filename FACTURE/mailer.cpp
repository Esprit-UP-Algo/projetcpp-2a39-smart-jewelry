#include "mailer.h"
#include <QtNetwork>
#include <QDebug>
#include "connection.h"
mailer::mailer()
{

}
int mailer::sendEmail(const QString& recipientEmail, const QString& body, const QString& nomFournisseur, const QString& prenomFournisseur)
{

    qDebug()<<"sslLibraryBuildVersionString: "<<QSslSocket::sslLibraryBuildVersionString();
    qDebug()<<"sslLibraryVersionNumber: "<<QSslSocket::sslLibraryVersionNumber();
     qDebug()<<"supportsSsl: "<<QSslSocket::supportsSsl();;
    qDebug()<<QCoreApplication::libraryPaths();
        // SMTP server information
        QString smtpServer = "smtp.gmail.com";
        int smtpPort = 465;  // Adjust this based on your SMTP server configuration
        QString username = "alloucheemna0@gmail.com";
        QString password = "bowi pmvv bvla ikqo";

        // Sender and recipient information
        QString from = "alloucheemna0@gmail.com";
        //QString to = "emna.allouche@esprit.tn";
        QString subject = "REMINDER";


        // Create a TCP socket
        QSslSocket socket;

        // Connect to the SMTP server
        socket.connectToHostEncrypted(smtpServer, smtpPort);
        if (!socket.waitForConnected()) {
            qDebug() << "Error connecting to the server:" << socket.errorString();
            return -1;
        }

        // Wait for the greeting from the server
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        qDebug() << "Connected to the server.";

        // Send the HELO command
        socket.write("HELO localhost\r\n");
        socket.waitForBytesWritten();

        // Read the response from the server
        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        // Send the authentication information
        socket.write("AUTH LOGIN\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        // Send the username
        socket.write(QByteArray().append(username.toUtf8()).toBase64() + "\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        // Send the password
        socket.write(QByteArray().append(password.toUtf8()).toBase64() + "\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        // Send the MAIL FROM command
        socket.write("MAIL FROM:<" + from.toUtf8() + ">\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        // Send the RCPT TO command
        socket.write("RCPT TO:<" + recipientEmail.toUtf8() + ">\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }


        socket.write("DATA\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }


        socket.write("From: " + from.toUtf8() + "\r\n");
        socket.write("To: " + recipientEmail.toUtf8() + "\r\n");
        socket.write("Subject: " + subject.toUtf8() + "\r\n");
        socket.write("\r\n");  // Empty line before the body
        socket.write(body.toUtf8() + "\r\n");
        socket.write(".\r\n");  // End of email content
        socket.write(body.toUtf8() + "\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }


        socket.write("QUIT\r\n");
        socket.waitForBytesWritten();

        if (!socket.waitForReadyRead()) {
            qDebug() << "Error reading from server:" << socket.errorString();
            return -1;
        }

        qDebug() << "Email sent successfully.";


        socket.close();
}
QString mailer::fetchRecipientEmailFromDatabase(int id)
{
    QString recipientEmail;

    QSqlQuery query;
    QString id_string = QString::number(id);
    query.prepare("SELECT COUNT(*) FROM FOURNISSEUR WHERE IDF = :idf");
    query.bindValue(":idf", id_string);


        query.prepare("SELECT EMAIL FROM FOURNISSEUR WHERE IDF =:id");
        query.bindValue(0, id);

        if (query.exec() && query.next()) {
            recipientEmail = query.value(0).toString();
    }
    return recipientEmail;
}

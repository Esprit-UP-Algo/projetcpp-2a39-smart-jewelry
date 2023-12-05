#ifndef SMS_H
#define SMS_H
#include "QString"


class sms
{
public:
    sms();
    void sendSMS(const QString &to, const QString &message);
};

#endif // SMS_H

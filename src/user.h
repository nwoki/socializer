/*
 * user.h
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef SOCIALIZER_USER_H
#define SOCIALIZER_USER_H

#include <QtCore/QString>


namespace Socializer {

class User
{
public:
//     User();
    User(const QString &id = QString()
        , const QString &firstName = QString()
        , const QString &surname = QString()
        , const QString &email = QString());
    ~User();

    QString email() const;
    QString firstName() const;
    QString id() const;
    QString surname() const;

    void setEmail(const QString &email);
    void setFirstName(const QString &firstName);
    void setId(const QString &id);
    void setSurname(const QString &surname);

private:
    class Private;
    Private * const d;
};

}       // Socializer



#endif  // SOCIALIZER_USER_H



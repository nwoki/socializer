/*
 * user.h
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef SOCIALIZER_USER_H
#define SOCIALIZER_USER_H

#include <QtCore/QDate>
#include <QtCore/QString>


namespace Socializer {

class User
{
public:
    User(const QString &id = QString()
        , const QString &firstName = QString()
        , const QString &lastName = QString()
        , const QString &email = QString()
        , const QDate &birthday = QDate());
    ~User();

    QDate birthday() const;
    QString email() const;
    QString firstName() const;
    QString id() const;
    QString lastName() const;

    void setBirthday(const QDate &birthday);
    void setEmail(const QString &email);
    void setFirstName(const QString &firstName);
    void setId(const QString &id);
    void setLastName(const QString &lastName);

private:
    class Private;
    Private * const d;
};

}       // Socializer



#endif  // SOCIALIZER_USER_H



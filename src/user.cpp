#include "user.h"

namespace Socializer {

class User::Private
{
public:
    Private() {};

    QDate birthday;
    QString email;
    QString firstName;
    QString id;
    QString lastName;
};


User::User(const QString &id, const QString &firstName, const QString &surname, const QString &email, const QDate &birthday)
    : d(new Private)
{
    d->birthday = birthday;
    d->email = email;
    d->firstName = firstName;
    d->id = id;
    d->email = email;
}

User::~User()
{
    delete d;
}

QDate User::birthday() const
{
    return d->birthday;
}

QString User::email() const
{
    return d->email;
}

QString User::firstName() const
{
    return d->firstName;
}

QString User::id() const
{
    return d->id;
}

void User::setBirthday(const QDate &birthday)
{
    d->birthday = birthday;
}

void User::setEmail(const QString &email)
{
    d->email = email;
}

void User::setFirstName(const QString &firstName)
{
    d->firstName = firstName;
}

void User::setId(const QString &id)
{
    d->id = id;
}

void User::setLastName(const QString &lastName)
{
    d->lastName = lastName;
}

QString User::lastName() const
{
    return d->lastName;
}


}

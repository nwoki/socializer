#include "user.h"

namespace Socializer {

class User::Private
{
public:
    Private();

    QString email;
    QString firstName;
    QString id;
    QString surname;
};


User::User(const QString &id, const QString &firstName, const QString &surname, const QString &email)
    : d(new Private)
{
    d->email = email;
    d->firstName = firstName;
    d->id = id;
    d->email = email;
}

User::~User()
{
    delete d;
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

void User::setSurname(const QString &surname)
{
    d->surname = surname;
}

QString User::surname() const
{
    return d->surname;
}





















}

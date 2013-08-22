/*
 * This file is part of the Socializer lib
 * Copyright (C) 2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */

#include "facebookuser.h"

namespace Socializer {

class FacebookUser::Private {
public:
    Private() {};

    QString id;
    QString bio;
    QString name;
    QString firstName;
    QString lastName;
    QString email;
    QString link;
    QString username;
    QDate birthday;
    QString gender;
    QString relationshipStatus;
    QString picture;
    QString verified;
    QString status;         // last status message displayed by user
    QString locale;
    QPair<QString, QString> location;
    QPair<QString, QString> hometown;
};


FacebookUser::FacebookUser(QObject *parent)
    : QObject(parent)
    , d(new Private)
{
    d->birthday = QDate::fromString("01-01-1970", "dd-MM-yyyy");
}


FacebookUser::~FacebookUser()
{
}


QString FacebookUser::bio() const
{
    return d->bio;
}


QDate FacebookUser::birthday() const
{
    return d->birthday;
}


QString FacebookUser::email() const
{
    return d->email;
}


QString FacebookUser::firstName() const
{
    return d->firstName;
}


QString FacebookUser::gender() const
{
    return d->gender;
}


QPair<QString, QString> &FacebookUser::hometown() const
{
    return d->hometown;
}


QString FacebookUser::id() const
{
    return d->id;
}


QString FacebookUser::lastName() const
{
    return d->lastName;
}


QString FacebookUser::link() const
{
    return d->link;
}


QString FacebookUser::locale() const
{
    return d->locale;
}


QPair<QString, QString> &FacebookUser::location() const
{
    return d->location;
}


QString FacebookUser::name() const
{
    return d->name;
}


QString FacebookUser::picture() const
{
    return d->picture;
}


QString FacebookUser::relationshipStatus() const
{
    return d->relationshipStatus;
}


QString FacebookUser::status() const
{
    return d->status;
}


QString FacebookUser::username() const
{
    return d->username;
}


QString FacebookUser::verified() const
{
    return d->verified;
}


void FacebookUser::setBio(const QString &bio)
{
    d->bio = bio;
}


void FacebookUser::setBirthday(const QDate& birthday)
{
    d->birthday = birthday;
}


void FacebookUser::setEmail(const QString &email)
{
    d->email = email;
}


void FacebookUser::setFirstName(const QString &firstName)
{
    d->firstName = firstName;
}


void FacebookUser::setGender(const QString &gender)
{
    d->gender = gender;
}


void FacebookUser::setId(const QString &id)
{
    d->id = id;
}



void FacebookUser::setLastName(const QString &lastName)
{
    d->lastName = lastName;
}


void FacebookUser::setLink(const QString &link)
{
    d->link = link;
}



void FacebookUser::setLocale(const QString &locale)
{
    d->locale = locale;
}


void FacebookUser::setName(const QString &name)
{
    d->name = name;
}


void FacebookUser::setPicture(const QString &picture)
{
    d->picture = picture;
}


void FacebookUser::setRelationshipStatus(const QString &relationshipStatus)
{
    d->relationshipStatus = relationshipStatus;
}


void FacebookUser::setStatus(const QString &status)
{
    d->status = status;
}


void FacebookUser::setUsername(const QString &username)
{
    d->username = username;
}


void FacebookUser::setVerified(const QString &verified)
{
    d->verified = verified;
}



};
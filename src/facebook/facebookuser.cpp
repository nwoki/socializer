/*
 * facebookuser.cpp
 *
 * This file is part of the Socializer lib
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "facebookuser.h"

#include <QtCore/QDebug>

namespace Socializer {

class FacebookUser::Private {
public:
    Private() {};

    QString bio;
    QString name;
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
    QString lastUpdatedTime;

    QList<Education> education;       /** List of the users education history */
    QHash<QString, Like> likes;       /** List of the users "likes" */        // key is ID
    QHash<QString, Friend> friends;   /** List of users friends */            // key is ID
    QList<Work> work;                 /** List of the users job history */
};


FacebookUser::FacebookUser(QObject *parent)
    : QObject(parent)
    , User()
    , d(new Private)
{
    d->birthday = QDate::fromString("01-01-1970", "dd-MM-yyyy");
}


FacebookUser::~FacebookUser()
{
}


void FacebookUser::addEducation(const FacebookUser::Education &education)
{
    if (d->education.contains(education)) {
        return;
    } else {
        d->education.append(education);
    }
}


void FacebookUser::addLike(const QString &id, const FacebookUser::Like &like)
{
    d->likes.insert(id, like);
}


void FacebookUser::addFriend(const QString &id, const FacebookUser::Friend &fbFriend)
{
    d->friends.insert(id, fbFriend);
}


void FacebookUser::addWork(const FacebookUser::Work &work)
{
    if (d->work.contains(work)) {
        return;
    } else {
        d->work.append(work);
    }
}


QString FacebookUser::bio() const
{
    return d->bio;
}

QList<FacebookUser::Education> FacebookUser::education() const
{
    return d->education;
}

QString FacebookUser::gender() const
{
    return d->gender;
}

QPair<QString, QString> &FacebookUser::hometown() const
{
    return d->hometown;
}

QString FacebookUser::lastUpdatedTime() const
{
    return d->lastUpdatedTime;
}


QHash<QString, FacebookUser::Like> FacebookUser::likes() const
{
    return d->likes;
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


QHash<QString, FacebookUser::Friend> FacebookUser::friends() const
{
    return d->friends;
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


QList<FacebookUser::Work> FacebookUser::work() const
{
    return d->work;
}


void FacebookUser::setBio(const QString &bio)
{
    d->bio = bio;
}

void FacebookUser::setGender(const QString &gender)
{
    d->gender = gender;
}

void FacebookUser::setLastUpdatedTime(const QString &lastUpdate)
{
    d->lastUpdatedTime = lastUpdate;
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

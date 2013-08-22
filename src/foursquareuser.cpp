/*
 * This file is part of the Socializer lib
 * Copyright (C) 2012-2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */

#include "foursquareuser.h"

namespace Socializer {

class FoursquareUser::Private {
public:
    Private() {}

    QString id;
    QString firstName;
    QString lastName;
    QString gender;
    QString relationship;
    QString photo;
    QString homecity;
    QString bio;
    qint16  totalCheckins;

    Contact contact;
    Venue lastCheckin;
    QList<Venue> checkins;
};

FoursquareUser::FoursquareUser(const QString& id, const QString& firstName, const QString& lastName, const QString gender
                            , const QString& relationship, const QString& homecity, const QString bio, QObject *parent)
    : QObject(parent)
    , d(new Private)
{
    d->id = id;
    d->firstName = firstName;
    d->lastName = lastName;
    d->gender = gender;
    d->relationship = relationship;
    d->homecity = homecity;
    d->bio = bio;
    d->totalCheckins = 0;

    d->lastCheckin.latitude = 0.0;
    d->lastCheckin.longitude = 0.0;
    d->lastCheckin.isMayor = false;
    d->lastCheckin.verified = false;
}


FoursquareUser::FoursquareUser(QObject *parent)
    : QObject(parent)
    , d(new Private)
{
    d->totalCheckins = 0;

    d->lastCheckin.latitude = 0.0;
    d->lastCheckin.longitude = 0.0;
    d->lastCheckin.isMayor = false;
    d->lastCheckin.verified = false;
}



FoursquareUser::~FoursquareUser()
{
    d->checkins.clear();

    delete d;
}


void FoursquareUser::addCheckin(const FoursquareUser::Venue &checkin)
{
    d->checkins.append(checkin);
}


QString FoursquareUser::bio() const
{
    return d->bio;
}


FoursquareUser::Contact &FoursquareUser::contact() const
{
    return d->contact;
}


QString FoursquareUser::firstName() const
{
    return d->firstName;
}


QString FoursquareUser::gender() const
{
    return d->gender;
}


QString FoursquareUser::homecity() const
{
    return d->homecity;
}


QString FoursquareUser::id() const
{
    return d->id;
}


FoursquareUser::Venue &FoursquareUser::lastCheckin() const
{
    return d->lastCheckin;
}


QString FoursquareUser::lastName() const
{
    return d->lastName;
}


QString FoursquareUser::photo() const
{
    return d->photo;
}


QString FoursquareUser::relationship() const
{
    return d->relationship;
}


quint16 FoursquareUser::totalCheckins() const
{
    return d->totalCheckins;
}


void FoursquareUser::setBio(const QString &bio)
{
    d->bio = bio;
}


void FoursquareUser::setFirstName(const QString &firstName)
{
    d->firstName = firstName;
}


void FoursquareUser::setGender(const QString &gender)
{
    d->gender = gender;
}


void FoursquareUser::setHomeCity(const QString &homecity)
{
    d->homecity = homecity;
}


void FoursquareUser::setId(const QString &id)
{
    d->id = id;
}


void FoursquareUser::setLastName(const QString &lastName)
{
    d->lastName = lastName;
}


void FoursquareUser::setPhoto(const QString &photo)
{
    d->photo = photo;
}


void FoursquareUser::setRelationship(const QString &relationship)
{
    d->relationship = relationship;
}


void FoursquareUser::setTotalCheckins(quint16 totalCheckins)
{
    d->totalCheckins = totalCheckins;
}

};      // socializer



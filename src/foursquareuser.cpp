/*
 * This file is part of the Socializer lib
 * Copyright (C) 2012-2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "foursquareuser.h"

using namespace Socializer;

FoursquareUser::FoursquareUser(const QString& id, const QString& firstName, const QString& lastName, const QString gender
                            , const QString& relationship, const QString& homecity, const QString bio, QObject *parent)
    : QObject(parent)
    , m_id(id)
    , m_firstName(firstName)
    , m_lastName(lastName)
    , m_gender(gender)
    , m_relationship(relationship)
    , m_homecity(homecity)
    , m_bio(bio)
    , m_lastCheckin(0)
{
}


FoursquareUser::FoursquareUser(QObject *parent)
    : QObject(parent)
    , m_lastCheckin(0)
{
}



FoursquareUser::~FoursquareUser()
{
    delete m_contact;
    delete m_lastCheckin;

    qDeleteAll(m_checkins);
    m_checkins.clear();
}


void FoursquareUser::addCheckin(FoursquareUser::Venue *checkin)
{
    qDebug("[FoursquareUser::addCheckin]");

    m_checkins.append(checkin);
}


QString FoursquareUser::bio() const
{
    qDebug("[FoursquareUser::bio]");

    return m_bio;
}


FoursquareUser::Contact *FoursquareUser::contact() const
{
    qDebug("[FoursquareUser::contact]");

    return m_contact;
}


QString FoursquareUser::firstName() const
{
    qDebug("[FoursquareUser::firstName]");

    return m_firstName;
}


QString FoursquareUser::gender() const
{
    qDebug("[FoursquareUser::gender]");

    return m_gender;
}


QString FoursquareUser::homecity() const
{
    qDebug("[FoursquareUser::homecity]");

    return m_homecity;
}


QString FoursquareUser::id() const
{
    qDebug("[FoursquareUser::homecity]");

    return m_id;
}


FoursquareUser::Venue* FoursquareUser::lastCheckin() const
{
    qDebug("[FoursquareUser::lastCheckin]");

    return m_lastCheckin;
}


QString FoursquareUser::lastName() const
{
    qDebug("[FoursquareUser::lastName]");

    return m_lastName;
}


QString FoursquareUser::photo() const
{
    qDebug("[FoursquareUser::photo]");

    return m_photo;
}


QString FoursquareUser::relationship() const
{
    qDebug("[FoursquareUser::relationship]");

    return m_relationship;
}


void FoursquareUser::setBio(const QString& bio)
{
    qDebug("[FoursquareUser::setBio]");

    m_bio = bio;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setFirstName(const QString &firstName)
{
    qDebug("[FoursquareUser::setFirstName]");

    m_firstName = firstName;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setGender(const QString &gender)
{
    qDebug("[FoursquareUser::setGender]");

    m_gender = gender;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setHomeCity(const QString &homecity)
{
    qDebug("[FoursquareUser::setHomeCity]");

    m_homecity = homecity;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setId(const QString &id)
{
    qDebug("[FoursquareUser::setId]");

    m_id = id;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setLastCheckin(FoursquareUser::Venue* lastCheckin)
{
    qDebug("[FoursquareUser::setLastCheckin]");

    if (m_lastCheckin != 0) {
        Venue *toDel = m_lastCheckin;
        delete toDel;
    }

    m_lastCheckin = lastCheckin;

    Q_EMIT userInfoChanged();
}


void FoursquareUser::setLastName(const QString &lastName)
{
    qDebug("[FoursquareUser::setLastName]");

    m_lastName = lastName;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setPhoto(const QString &photo)
{
    qDebug("[FoursquareUser::setPhoto]");

    m_photo = photo;
    Q_EMIT userInfoChanged();
}


void FoursquareUser::setRelationship(const QString &relationship)
{
    qDebug("[FoursquareUser::setRelationship]");

    m_relationship = relationship;
    Q_EMIT userInfoChanged();
}




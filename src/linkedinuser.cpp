/*
 * linkedinuser.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "linkedinuser.h"

namespace Socializer {

class LinkedInUser::Private
{
public:
    Private() {}

    QString profileId;
    QString email;
    QString firstName;
    QString headline;
    QString lastName;
    quint16 numOfConnections;
    quint16 numOfRecommenders;
    QString specialities;
    QString pictureUrl;
    QString interests;
    QString location;
    QString locationCountryCode;
    QString industry;
    QString profilePictureUrl;

    QHash<QString, Position> currentPositions;        // id:Position
    QHash<QString, Language> languages;               // id:Language
    QHash<QString, QString> skills;                   // id:name
    QHash<QString, Certification> certifications;     // id:certification
    QHash<QString, Education> education;              // id:education
    QHash<QString, Recommendation> recommendations;   // id:recommendation
};

LinkedInUser::LinkedInUser(QObject *parent)
    : QObject(parent)
    , d(new Private)
{
    d->numOfConnections = 0;
    d->numOfRecommenders = 0;
}


LinkedInUser::~LinkedInUser()
{
}


QString LinkedInUser::email() const
{
    return d->email;
}


QString LinkedInUser::firstName() const
{
    return d->firstName;
}


QString LinkedInUser::headline() const
{
    return d->headline;
}


QString LinkedInUser::industry() const
{
    return d->industry;
}


QHash<QString, LinkedInUser::Language> LinkedInUser::languages() const
{
    return d->languages;
}


QString LinkedInUser::lastName() const
{
    return d->lastName;
}


QString LinkedInUser::location() const
{
    return d->location;
}


QString LinkedInUser::locationCountryCode() const
{
    return d->locationCountryCode;
}


quint16 LinkedInUser::numberOfConnections() const
{
    return d->numOfConnections;
}


quint16 LinkedInUser::numberOfRecommenders() const
{
    return d->numOfRecommenders;
}


QString LinkedInUser::profileId() const
{
    return d->profileId;
}


QString LinkedInUser::profilePictureUrl() const
{
    return d->profilePictureUrl;
}


void LinkedInUser::addLanguage(const QString &id, const LinkedInUser::Language &lang)
{
    d->languages.insert(id, lang);
}


void LinkedInUser::setEmail(const QString &data)
{
    d->email = data;
}


void LinkedInUser::setFirstName(const QString &data)
{
    d->firstName = data;
}


void LinkedInUser::setHeadLine(const QString& data)
{
    d->headline = data;
}


void LinkedInUser::setIndustry(const QString& data)
{
    d->industry = data;
}


void LinkedInUser::setLastName(const QString &data)
{
    d->lastName = data;
}


void LinkedInUser::setLocation(const QString &data)
{
    d->location = data;
}


void LinkedInUser::setLocationCountryCode(const QString &data)
{
    d->locationCountryCode = data;
}


void LinkedInUser::setNumberOfConnections(quint16 connections)
{
    d->numOfConnections = connections;
}


void LinkedInUser::setNumberOfRecommenders(quint16 recommenders)
{
    d->numOfRecommenders = recommenders;
}


void LinkedInUser::setProfileId(const QString &data)
{
    d->profileId = data;
}


void LinkedInUser::setProfilePictureUrl(const QString& data)
{
    d->profilePictureUrl = data;
}


}       // socializer



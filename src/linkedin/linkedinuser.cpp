/*
 * linkedinuser.cpp
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
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
    QDate birthday;
    QString publicProfileUrl;
    QString lastUpdatedTime;                            // string with the last time the profile was edited

    QHash<QString, Position> positions;                 // id:Position
    QHash<QString, Language> languages;                 // id:Language
    QHash<QString, QString> skills;                     // id:name
    QHash<QString, Certification> certifications;       // id:certification
    QHash<QString, Education> education;                // id:education
    QHash<QString, Recommendation> recommendations;     // id:recommendation
    QHash<QString, Group> groups;                       // id:group
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


QDate LinkedInUser::birthday() const
{
    return d->birthday;
}


QHash<QString, LinkedInUser::Education> LinkedInUser::educaitons() const
{
    return d->education;
}


QString LinkedInUser::email() const
{
    return d->email;
}


QString LinkedInUser::firstName() const
{
    return d->firstName;
}


QHash<QString, LinkedInUser::Group> LinkedInUser::groups() const
{
    return d->groups;
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


QString LinkedInUser::lastUpdatedTime() const
{
    return d->lastUpdatedTime;
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


QHash< QString, LinkedInUser::Position > LinkedInUser::positions() const
{
    return d->positions;
}


QString LinkedInUser::profileId() const
{
    return d->profileId;
}


QString LinkedInUser::profilePictureUrl() const
{
    return d->profilePictureUrl;
}


QHash<QString, LinkedInUser::Recommendation> LinkedInUser::recommendations() const
{
    return d->recommendations;
}


QHash<QString, QString> LinkedInUser::skills() const
{
    return d->skills;
}


void LinkedInUser::addEducation(const QString &id, const LinkedInUser::Education &education)
{
    d->education.insert(id, education);
}


void LinkedInUser::addGroup(const QString &id, const LinkedInUser::Group &group)
{
    d->groups.insert(id, group);
}


void LinkedInUser::addLanguage(const QString &id, const LinkedInUser::Language &lang)
{
    d->languages.insert(id, lang);
}


void LinkedInUser::addPosition(const QString &id, const LinkedInUser::Position &position)
{
    d->positions.insert(id, position);
}


void LinkedInUser::addRecommendation(const QString &id, const LinkedInUser::Recommendation &recommendation)
{
    d->recommendations.insert(id, recommendation);
}


void LinkedInUser::addSkill(const QString &id, const QString &skill)
{
    d->skills.insert(id, skill);
}


void LinkedInUser::setBirthday(const QDate &birthday)
{
    d->birthday = birthday;
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


void LinkedInUser::setLastUpdatedTime(const QString &data)
{
    d->lastUpdatedTime = data;
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


QString LinkedInUser::publicProfileUrl() const
{
    return d->publicProfileUrl;
}


void LinkedInUser::setPublicProfileUrl(const QString &data)
{
    d->publicProfileUrl = data;
}




}       // socializer



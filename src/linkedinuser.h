/*
 * linkedinuser.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012-2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */


#ifndef LINKEDINUSER_H
#define LINKEDINUSER_H

#include <QtCore/QDate>
#include <QtCore/QObject>

namespace Socializer {

class LinkedInUser : public QObject
{
    Q_OBJECT

public:
    struct Language {
        QString language;
        QString proficiencyLevel;
        QString proficiencyName;
    };

    struct Company {
        QString id;
        QString name;
        QString type;
        QString size;
        QString industry;
        QString ticker;
    };

    struct Position {
        // the key of the hash is the id
        QString title;
        QString summary;
        QDate startDate;
        QDate endDate;
        bool isCurrent;
        Company company;
    };

    struct Certification {
        QString name;
        QString authority;
        QString number;
        QDate startDate;
        QDate endDate;
    };

    struct Education {
        QString schoolName;
        QString fieldOfStudy;
        QString startDate;
        QString endDate;
        QString degree;
        QString activities;
        QString notes;
    };

    struct Recommender {
        QString id;
        QString firstName;
        QString lastName;
    };

    struct Recommendation {
        QString type;
        QString text;
        Recommender recommender;
    };

    LinkedInUser(QObject *parent = 0);
    ~LinkedInUser();

    QHash<QString, Education> educaitons() const;
    QString email() const;
    QString firstName() const;
    QString headline() const;
    QString industry() const;
    QHash<QString, Language> languages() const;
    QString lastName() const;
    QString location() const;
    QString locationCountryCode() const;
    quint16 numberOfConnections() const;
    quint16 numberOfRecommenders() const;
    QHash<QString, Position> positions() const;
    QString profileId() const;
    QString profilePictureUrl() const;
    QHash<QString, Recommendation> recommendations() const;
    QHash<QString, QString> skills() const;

    void addEducation(const QString &id, const Education &education);
    void addLanguage(const QString &id, const Language &lang);
    void addPosition(const QString &id, const Position &position);
    void addRecommendation(const QString &id, const Recommendation &recommendation);
    void addSkill(const QString &id, const QString &skill);
    void setEmail(const QString &data);
    void setFirstName(const QString &data);
    void setHeadLine(const QString &data);
    void setIndustry(const QString &data);
    void setLastName(const QString &data);
    void setLocation(const QString &data);
    void setLocationCountryCode(const QString &data);
    void setNumberOfConnections(quint16 connections);
    void setNumberOfRecommenders(quint16 recommenders);
    void setProfileId(const QString &data);
    void setProfilePictureUrl(const QString &data);


private:
    class Private;
    Private * const d;
};

};      // socializer

#endif // LINKEDINUSER_H

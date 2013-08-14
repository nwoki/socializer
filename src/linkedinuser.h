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

class LinkedInUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString email            READ email      WRITE setEmail          NOTIFY infoChanged)
    Q_PROPERTY(QString firstName        READ firstName  WRITE setFirstName      NOTIFY infoChanged)
    Q_PROPERTY(QString headline         READ headline   WRITE setHeadLine       NOTIFY infoChanged)
    Q_PROPERTY(QString lastName         READ lastName   WRITE setLastName       NOTIFY infoChanged)

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
        quint16 size;
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
        QDate startDate;
        QDate endDate;
        QString degree;
        QString activities;
        QString notes;
    };

    struct Recommendation {
        QString type;
        QString text;
        QString recommender;
    };

    LinkedInUser(QObject *parent = 0);
    ~LinkedInUser();

    QString email() const;
    QString firstName() const;
    QString headline() const;
    QString lastName() const;

    void setEmail(const QString &data);
    void setFirstName(const QString &data);
    void setHeadLine(const QString &data);
    void setLastName(const QString &data);

Q_SIGNALS:
    void infoChanged();

private:
    QString m_email;
    QString m_firstName;
    QString m_headline;
    QString m_lastName;
    quint16 m_numOfConnections;
    QString m_specialities;
    QString m_pictureUrl;
    QString m_interests;

    QHash<QString, Position> m_currentPositions;        // id:Position
    QHash<QString, Language> m_languages;               // id:Language
    QHash<QString, QString> m_skills;                   // id:name
    QHash<QString, Certification> m_certifications;     // id:certification
    QHash<QString, Education> m_education;              // id:education
    QHash<QString, Recommendation> m_recommendations;   // id:recommendation
};

#endif // LINKEDINUSER_H

    struct C;

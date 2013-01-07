/*
 * This file is part of the Socializer lib
 * Copyright (C) 2012-2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef FOURSQUAREUSER_H
#define FOURSQUAREUSER_H

#include <QtCore/QObject>
#include <QtCore/QString>

class FoursquareUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY (QString id              READ id                 NOTIFY userInfoChanged)
    Q_PROPERTY (QString firstName       READ firstName          NOTIFY userInfoChanged)
    Q_PROPERTY (QString lastName        READ lastName           NOTIFY userInfoChanged)
    Q_PROPERTY (QString gender          READ gender             NOTIFY userInfoChanged)
    Q_PROPERTY (QString relationship    READ relationship       NOTIFY userInfoChanged)
    Q_PROPERTY (QString homecity        READ homecity           NOTIFY userInfoChanged)
    Q_PROPERTY (QString bio             READ bio                NOTIFY userInfoChanged)
    Q_PROPERTY (Contact*contact         READ contact            NOTIFY userInfoChanged)

public:

    struct Contact {
        QString facebook;
        QString email;
        QString twitter;
        QString phone;
    };

    struct Venue {
        QString id;
        QString name;
        QString address;
        double latitude;
        double longitude;
        QString postalCode;
        QString city;
        QString state;
        QString country;
        QString cc;
    };

    FoursquareUser(const QString &id, const QString &firstName, const QString &lastName
                , const QString gender, const QString &relationship, const QString &homecity
                , const QString bio, QObject *parent = 0);

    FoursquareUser(QObject *parent = 0);
    ~FoursquareUser();

    QString id() const;
    QString firstName() const;
    QString lastName() const;
    QString gender() const;
    QString relationship() const;
    QString homecity() const;
    QString bio() const;

    Contact *contact() const;

    void addCheckin(Venue *checkin);
    void setId(const QString &id);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setGender(const QString &gender);
    void setRelationship(const QString &relationship);
    void setHomeCity(const QString &homecity);
    void setBio(const QString &bio);

Q_SIGNALS:
    void userInfoChanged();

private:
    QString m_id;
    QString m_firstName;
    QString m_lastName;
    QString m_gender;
    QString m_relationship;
    QString m_photo;
    QString m_homecity;
    QString m_bio;

    Contact* m_contact;

    QList<Venue*> m_checkins;
};

#endif // FOURSQUAREUSER_H
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


namespace Socializer {

class FoursquareUser : public QObject
{
    Q_OBJECT

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
        QString canonicalUrl;
        bool verified;
        bool isMayor;
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
    QString photo() const;
    quint16 totalCheckins() const;

    Contact &contact() const;
    Venue &lastCheckin() const;

    void addCheckin(const Venue &checkin);
    void setId(const QString &id);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setGender(const QString &gender);
    void setRelationship(const QString &relationship);
    void setHomeCity(const QString &homecity);
    void setBio(const QString &bio);
    void setPhoto(const QString &photo);
    void setTotalCheckins(quint16 totalCheckins);

private:
    class Private;
    Private * const d;
};

};

#endif // FOURSQUAREUSER_H

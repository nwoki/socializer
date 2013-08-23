/*
 * This file is part of the Socializer lib
 * Copyright (C) 2013 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco.nwokeka@ispirata.com>
 */

#ifndef FACEBOOKUSER_H
#define FACEBOOKUSER_H

#include <QtCore/QDate>
#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QPair>
#include <QtCore/QString>


namespace Socializer {

class FacebookUser : public QObject
{
    Q_OBJECT

public:
    struct Like {
        QString category;
        QString name;
        QString id;
        QString createdTime;
    };

    FacebookUser(QObject *parent = 0);
    virtual ~FacebookUser();

    void addLike(const QString &id, const Like &like);

    QString id() const;
    QString bio() const;
    QString name() const;
    QString firstName() const;
    QString lastName() const;
    QString email() const;
    QHash<QString, Like> likes() const;
    QString link() const;
    QString username() const;
    QDate birthday() const;
    QString gender() const;
    QString relationshipStatus() const;
    QString picture() const;
    QString verified() const;
    QString status() const;         // last status message displayed by user
    QString locale() const;

    QPair<QString, QString> &location() const;
    QPair<QString, QString> &hometown() const;

    void setId(const QString &id);
    void setBio(const QString &bio);
    void setName(const QString &name);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setEmail(const QString &email);
    void setLink(const QString &link);
    void setUsername(const QString &username);
    void setBirthday(const QDate &birthday);
    void setGender(const QString &gender);
    void setRelationshipStatus(const QString &relationshipStatus);
    void setPicture(const QString &picture);
    void setVerified(const QString &verified);
    void setStatus(const QString &status);         // last status message displayed by user
    void setLocale(const QString &locale);

private:
    class Private;
    Private * const d;
};

};

#endif  // FACEBOOKUSER_H
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
    // TODO
    // game
    //     struct Game {
    //
    //     };
    //
    //     // movie
    //     struct Movie {
    //
    //     };
    //
    //     // music
    //     struct Music {
    //
    //     };

    struct Education {
        QPair<QString, QString> school;
        QString type;

        bool operator== (const Education &other) {
            if (school.first != other.school.first || school.second != other.school.second) {
                return false;
            } else if (type != other.type) {
                return false;
            } else {
                return true;
            }
        }
    };

    struct Like {
        QString category;
        QString name;
        QString id;
        QString createdTime;
    };

    struct Friend {
        QString id;
        QString name;
        QString firstName;
        QString lastName;
        QString gender;
        QString picture;
        QString locale;
        QString username;
        QString link;
        QString relationshipStatus;
        QString about;
        QString birthday;
    };

    struct Work {
        QPair<QString, QString> employer;
        QPair<QString, QString> location;
        QPair<QString, QString> position;
        QString description;
        QDate startDate;
        QDate endDate;

        bool operator== (const Work &other) const {
            // gen values check
            if (description != other.description || startDate != other.startDate || endDate != other.endDate) {
                return false;
            } else if (employer.first != other.employer.first || employer.second != other.employer.second) {
                return false;
            } else if (location.first != other.location.first || location.second != other.location.second) {
                return false;
            } else if (position.first != other.position.first || position.second != other.position.second) {
                return false;
            } else {
                return true;
            }
        }
    };

    FacebookUser(QObject *parent = 0);
    virtual ~FacebookUser();

    void addEducation(const Education &education);
    void addLike(const QString &id, const Like &like);
    void addFriend(const QString &id, const Friend &fbFriend);
    void addWork(const Work &work);

    QString bio() const;
    QDate birthday() const;
    QList<Education> education() const;
    QString email() const;
    QString id() const;
    QString firstName() const;
    QHash<QString, Friend> friends() const;
    QString gender() const;
    QPair<QString, QString> &hometown() const;
    QString lastName() const;
    QHash<QString, Like> likes() const;
    QString link() const;
    QString locale() const;
    QPair<QString, QString> &location() const;
    QString name() const;
    QString picture() const;
    QString relationshipStatus() const;
    QString status() const;                     // last status message displayed by user
    QString verified() const;
    QList<Work> work() const;                   /** returns the user's list of work experiences */
    QString username() const;

    void setBio(const QString &bio);
    void setBirthday(const QDate &birthday);
    void setEmail(const QString &email);
    void setId(const QString &id);
    void setFirstName(const QString &firstName);
    void setGender(const QString &gender);
    void setLastName(const QString &lastName);
    void setLink(const QString &link);
    void setLocale(const QString &locale);
    void setName(const QString &name);
    void setPicture(const QString &picture);
    void setRelationshipStatus(const QString &relationshipStatus);
    void setStatus(const QString &status);         // last status message displayed by user
    void setVerified(const QString &verified);
    void setUsername(const QString &username);

private:
    class Private;
    Private * const d;
};

};

#endif  // FACEBOOKUSER_H
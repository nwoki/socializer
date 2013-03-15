/*
 * linkedinuser.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */


#ifndef LINKEDINUSER_H
#define LINKEDINUSER_H

#include <QtCore/QObject>

class LinkedInUser : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString email            READ email      WRITE setEmail          NOTIFY infoChanged)
    Q_PROPERTY(QString firstName        READ firstName  WRITE setFirstName      NOTIFY infoChanged)
    Q_PROPERTY(QString lastName         READ lastName   WRITE setLastName       NOTIFY infoChanged)

public:
    LinkedInUser(QObject *parent = 0);
    ~LinkedInUser();

    QString email() const;
    QString firstName() const;
    QString lastName() const;

    void setEmail(const QString &data);
    void setFirstName(const QString &data);
    void setLastName(const QString &data);

Q_SIGNALS:
    void infoChanged();

private:
    QString m_firstName;
    QString m_lastName;
    QString m_email;
};

#endif // LINKEDINUSER_H

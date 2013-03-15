/*
 * linkedinuser.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "linkedinuser.h"

LinkedInUser::LinkedInUser(QObject *parent)
    : QObject(parent)
{
}


LinkedInUser::~LinkedInUser()
{
}


QString LinkedInUser::email() const
{
    qDebug("[LinkedInUser::email]");

    return m_email;
}


QString LinkedInUser::firstName() const
{
    qDebug("[LinkedInUser::firstName]");

    return m_firstName;
}


QString LinkedInUser::headline() const
{
    qDebug("[LinkedInUser::headline]");

    return m_headline;
}


QString LinkedInUser::lastName() const
{
    qDebug("[LinkedInUser::lastName]");

    return m_lastName;
}


void LinkedInUser::setEmail(const QString &data)
{
    qDebug("[LinkedInUser::setEmail]");

    m_email = data;
    Q_EMIT infoChanged();
}


void LinkedInUser::setFirstName(const QString &data)
{
    qDebug("[LinkedInUser::setFirstName]");

    m_firstName = data;
    Q_EMIT infoChanged();
}


void LinkedInUser::setHeadLine(const QString& data)
{
    qDebug("[LinkedInUser::setHeadLine]");

    m_headline = data;
    Q_EMIT infoChanged();
}


void LinkedInUser::setLastName(const QString &data)
{
    qDebug("[LinkedInUser::setLastName]");

    m_lastName = data;
    Q_EMIT infoChanged();
}



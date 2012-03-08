/*
 * oauth.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "oauth.h"

#include <QtCore/QDateTime>
#include <QDebug>

#define NONCE_SIZE 16

using namespace Socializer;


OAuth::OAuth(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : QObject(parent)
    , m_appId(appId)
    , m_redirectUrl(redirectUrl)
{
}


OAuth::~OAuth()
{
}


QByteArray OAuth::appId() const
{
    return m_appId;
}


QByteArray OAuth::authToken() const
{
    return m_authToken;
}


QByteArray OAuth::timeStamp()
{
    return QByteArray::number(QDateTime::currentDateTime().toTime_t());
}


QByteArray OAuth::nonce()
{
    QByteArray chars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    int max = chars.size();

    QByteArray nonce;

    for(int i = 0; i < NONCE_SIZE; ++i){
        nonce.append(chars[qrand() % max]);
    }

    return nonce;
}


QByteArray OAuth::redirectUrl() const
{
    return m_redirectUrl;
}


void OAuth::setAppId(const QByteArray &appId)
{
    m_appId = appId;
}


void OAuth::setAuthToken(const QByteArray &authToken)
{
    m_authToken = authToken;
}


void OAuth::setRedirectUrl(const QByteArray &redirectUrl)
{
    m_redirectUrl = redirectUrl;
}





/*
 * linkedin.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "linkedin.h"

#include <QtCore/QDebug>
#include <QtNetwork/QNetworkAccessManager>

#define AUTHENTICATE_URL "https://api.linkedin.com/uas/oauth/authenticate"
#define REQUEST_URL "https://api.linkedin.com/uas/oauth/requestToken"

using namespace Socializer;


LinkedIn::LinkedIn(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject* parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
{
    connect(this, SIGNAL(requestTokenRecieved()), this, SLOT(prepareAuthPageUrl()));
}


LinkedIn::~LinkedIn()
{
}


void LinkedIn::obtainAuthPageUrl()
{
    if (m_authTokenSecret.isEmpty()) {
        qDebug("sending request cos auth is empty");
        obtainRequestToken(REQUEST_URL);
    } else {
        qDebug("requesting auth..");
        prepareAuthPageUrl();
    }
}


void LinkedIn::prepareAuthPageUrl()
{
    // once i have the access token, return the authentication url
    QString authPageUrl(AUTHENTICATE_URL);
    authPageUrl += "?oauth_token=" + m_authToken;

#ifdef DEBUG_MODE
    qDebug() << "[LinkedIn::prepareAuthPageUrl] Emitting auth page url: " << authPageUrl;
#endif

    Q_EMIT authPageUrlReady(authPageUrl);
}





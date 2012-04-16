/*
 * twitter.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "twitter.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#define AUTHENTICATE_URL "https://api.twitter.com/oauth/authenticate"
#define REQUEST_TOKEN_URL "https://api.twitter.com/oauth/request_token"

using namespace Socializer;


Twitter::Twitter(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
{
}


Twitter::~Twitter()
{
}


QString Twitter::obtainAuthPageUrl()
{
    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 2)

    // need access token first
    if (m_authToken.isEmpty()) {
        /// TODO notify error, we need to request the auth token first
        qDebug("[ERROR] Twitter::obtainAuthPageUrl: need to obtain request token first");
        obtainRequestToken(REQUEST_TOKEN_URL);
        return QString();
    }

    // once i have the access token, return the authentication url
    QString authPageUrl(AUTHENTICATE_URL);
    authPageUrl += "?auth_token=" + m_authToken;

    return authPageUrl;
}

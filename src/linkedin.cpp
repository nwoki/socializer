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

#define AUTHENTICATE_URL "https://www.linkedin.com/uas/oauth/authenticate"
#define REQUEST_URL "https://api.linkedin.com/uas/oauth/requestToken"

using namespace Socializer;

/**
 * You, the developer (aka the "consumer") requests an API (or consumer) key from us, LinkedIn (aka the "provider")
 A . When your application needs to authenticate the member (aka the "user"), your application makes a call to Link*edIn
    to ask for a request token
 B. LinkedIn replies with a request token. Request tokens are used to ask for user approval to the API.
 C. Your application redirects the member to LinkedIn to sign-in and authorize your application to make API calls on their behalf. You provide us with a URL where we should send them afterward (aka the "callback")
 D. If the member agrees, LinkedIn returns them to the location specified in the callback
 E. Your application then makes another OAuth call to LinkedIn to retrieve an access token for the member
 F. LinkedIn returns an access token, which has two parts: the oauth_token and oauth_token_secret.
 G. After retrieving the access token, you can make API calls, signing them with the consumer key and access token
 */

LinkedIn::LinkedIn(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject* parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
{
}


LinkedIn::~LinkedIn()
{
}


QString LinkedIn::obtainAuthPageUrl()
{
    if (m_authTokenSecret.isEmpty()) {
        qDebug("sending request cos auth is empty");
        obtainRequestToken(REQUEST_URL);
    } else {
        qDebug("requesting auth..");
    }

    return QString();
}




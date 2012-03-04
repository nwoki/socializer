/*
 * foursquare.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "foursquare.h"

#include <QtCore/QDebug>

#define AUTH_URL "https://foursquare.com/oauth2/authenticate?"

using namespace Socializer;


Foursquare::Foursquare(const QByteArray &appId, const QByteArray &redirectUrl)
    : m_oauth(new OAuth(appId, redirectUrl))
{
}


Foursquare::Foursquare(OAuth *oauth)
    : m_oauth(oauth)
{
}


Foursquare::~Foursquare()
{
}


QString Foursquare::obtainAuthPageUrl()
{
    QString urlStr(AUTH_URL);

    urlStr.append("client_id=");
    urlStr.append(m_oauth->appId());
    urlStr.append("&response_type=token");

    // there MUST be a redirect url value.
    /// TODO Check for empty parameter?
    urlStr.append("&redirect_uri=");
    urlStr.append(m_oauth->redirectUrl());

    return urlStr;
}


void Foursquare::setAuthToken(const QByteArray& token)
{
    m_oauth->setAuthToken(token);
}


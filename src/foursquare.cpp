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


Foursquare::Foursquare(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, QByteArray(), parent)
{
}


Foursquare::~Foursquare()
{
}


QString Foursquare::obtainAuthPageUrl()
{
    QString urlStr(AUTH_URL);

    urlStr.append("client_id=");
    urlStr.append(m_appId);
    urlStr.append("&response_type=token&redirect_uri=");
    urlStr.append(m_redirectUrl);

    return urlStr;
}

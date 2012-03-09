/*
 * twitter.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef TWITTER_H
#define TWITTER_H

#include "oauth.h"

#include <QtCore/QByteArray>
#include <QtCore/QPair>

class QNetworkAccessManager;
class QNetworkReply;

namespace Socializer
{

/**
 * Twitter interface
 */

class Twitter : public OAuth
{
    Q_OBJECT

public:
    enum RequestType {
        GET,
        POST
    };

    Twitter(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent = 0);
    ~Twitter();

    QString obtainAuthPageUrl();

    /** sends request to twitter service for access token to be used with Twitter::obtainAuthPage */
    void requestAccessToken();


private Q_SLOTS:
    void onReplyRecieved();

private:
    /**
     * generates the auth header for auth_token request
     * @param type the http method to generate the header for. Each method has different implementations
     * @param url the url to use for the request
     */
    QByteArray generateRequestAccessTokenHeader(RequestType type, QByteArray url);

    /**
     * generates the OAuth signature base to encrypt in HMAC-SHA1 later on
     * https://dev.twitter.com/docs/auth/creating-signature - Creating the signature base string
     */
    QByteArray generateBaseString(RequestType type, const QByteArray &url, QList<QPair<QByteArray, QByteArray> > params);

    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_networkReply;

    QByteArray m_consumerSecret;
};

};

#endif  // TWITTER_H
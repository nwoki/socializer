/*
 * oauth.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef OAUTH_H
#define OAUTH_H

#include <QtCore/QByteArray>
#include <QtCore/QObject>

/**
 * Class used to handle OAuth data for various services
 */

namespace Socializer
{

class OAuth : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArray appId READ appId WRITE setAppId)
    Q_PROPERTY(QByteArray authToken READ authToken WRITE setAuthToken)
    Q_PROPERTY(QByteArray redirectUrl READ redirectUrl WRITE setRedirectUrl)

public:
    /// TODO should redirectUrl be a needed param?
    OAuth(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent = 0);
    ~OAuth();

    QByteArray appId() const;
    QByteArray authToken() const;

    /**
     * encode in hmacsha1 the given string
     */
    QByteArray hmacsha1Encode(const QByteArray &baseStr, QByteArray key);

    /**
     * Generates a unique token for a request (Twitter). random 16 length string
     *
     * http://oauth.net/core/1.0/#nonce
     *
     * A nonce is a random string, uniquely generated for each request.
     * The nonce allows the Service Provider to verify that a request has never been made before
     * and helps prevent replay attacks when requests are made over a non-secure channel (such as HTTP).
     */
    QByteArray nonce();

    /** returns timestamp in epoch time according to the OAuth spec http://oauth.net/core/1.0/#nonce */
    QByteArray timeStamp();

    /** returns url to access for web page authentication */
    virtual QString obtainAuthPageUrl() = 0;

    QByteArray redirectUrl() const;

    void setAppId(const QByteArray &appId);
    void setAuthToken(const QByteArray &authToken);
    void setRedirectUrl(const QByteArray &redirectUrl);

private:
    QByteArray m_appId;         /** id code to authenticate service to (consumerKey/clientId) */
    QByteArray m_authToken;     /** auth token recieved from service */
    QByteArray m_redirectUrl;   /** url to redirect after auth */
};

};


#endif  // OAUTH_H
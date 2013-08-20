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
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QWeakPointer>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

/**
 * Class used to handle OAuth data for various services
 */

namespace Socializer
{

class OAuth : public QObject
{
    Q_OBJECT

//     Q_PROPERTY(QByteArray appId         READ appId          WRITE setAppId)
    Q_PROPERTY(QByteArray authToken     READ authToken      NOTIFY authTokenChanged)
    Q_PROPERTY(QByteArray redirectUrl   READ redirectUrl)

public:
    /// TODO should redirectUrl be a needed param?
    OAuth(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret = QByteArray(), QObject *parent = 0);
    OAuth(const QByteArray &authToken, QObject *parent = 0);
    ~OAuth();

//     QByteArray appId() const;
    QByteArray authToken() const;

    /**
     * generates the OAuth signature base to encrypt in HMAC-SHA1 later on
     * https://dev.twitter.com/docs/auth/creating-signature - Creating the signature base string
     */
    QByteArray generateBaseString(QNetworkAccessManager::Operation opType, const QByteArray &url, QList<QPair<QByteArray, QByteArray> > params);

    /**
     * generates the auth header for auth_token request
     * @param type the http method to generate the header for. Each method has different implementations
     * @param url the url to use for the request
     */
    QByteArray generateRequestHeader(QNetworkAccessManager::Operation opType, QByteArray url);

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
    virtual void obtainAuthPageUrl() = 0;


    /**
     * used to parse urls given by the QML Facebook component. It parses
     * the url to extract various errors and/or the auth token if all goes well
     * @param url the new url
     */
    virtual void parseNewUrl(const QString &url) = 0;


    /**
     * sends request to service for access token to be used with OAuth::obtainAuthPage
     * This is to be used for first access to the service or if the auth token has expired
     * @param requestUrl url to request for the token
     */
    void obtainRequestToken(const QByteArray &requestUrl);


    QByteArray redirectUrl() const;


    /**
     * sends a post request to the server converting the previously received request token
     * to a valid ACCESS TOKEN
     * @param url url to send the POST request to (twitter, linkedin, dropbox.. )
     * @param authVerifier auth verifier token received from the "obtainRequestToken" call
     */
    void requestAccessToken(const QByteArray &url ,const QByteArray &authVerifier);


//     void setAppId(const QByteArray &appId);
    void setAuthToken(const QByteArray &authToken);
//     void setRedirectUrl(const QByteArray &redirectUrl);

Q_SIGNALS:
    void authPageUrlReady(const QString &authPageUrl);
    void authTokenChanged();
    void requestTokenRecieved();

private Q_SLOTS:
    void onNetworkErrorRecieved(QNetworkReply::NetworkError error);
    void onObtainRequestTokenReplyRecieved();
    void onRequestAccessTokenReceived();

protected:
    QByteArray m_appId;             /** id code to authenticate service to (consumerKey/clientId) */
    QByteArray m_authToken;         /** auth token recieved from service */
    QByteArray m_authTokenSecret;   /** secret for the auth token recieved with the auth token upon request */
    QByteArray m_consumerSecret;
    QByteArray m_redirectUrl;       /** url to redirect after auth */
    QByteArray m_requestToken;
    QByteArray m_screenName;
    QByteArray m_userId;

    // network
    QNetworkAccessManager *m_networkAccessManager;

    /** extracts a QJsonObject from given json data */
    QJsonObject jsonObject(const QByteArray &jsonData);

private:
    QNetworkReply *m_networkReply;
};

};


#endif  // OAUTH_H

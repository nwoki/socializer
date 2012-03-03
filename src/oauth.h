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

class QNetworkAccessManager;

/**
 * Class used to recieve OAuth token from various services
 */

namespace Socializer
{

class OAuth : public QObject
{
    Q_PROPERTY(QByteArray appId READ appId WRITE setAppId)

public:
    OAuth();
    ~OAuth();



private:
//     /**
//      * Generates a unique token for a request
//      *
//      * The oauth_nonce parameter is a unique token your application
//      * should generate for each unique request. Twitter will use this
//      * value to determine whether a request has been submitted multiple times.
//      * The value for this request was generated by base64 encoding 32 bytes of
//      * random data, and stripping out all non-word characters, but any approach
//      * which produces a relatively random alphanumeric string should be OK here.
//      * */
//     QByteArray generateNonce();

    QByteArray m_appId;         /** id code to authenticate service to (consumerKey/clientId) */
    QByteArray m_authToken;     /** auth token recieved from service */
    QByteArray m_nonce;         /** a unique token the application should generate for each unique request */

    QNetworkAccessManager *m_accessManager;
};

};


#endif  // OAUTH_H
/*
 * oauth.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "oauth.h"


#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDateTime>
#include <QtCore/QDebug>

#include <QtNetwork/QNetworkRequest>

#define NONCE_SIZE 16

using namespace Socializer;


OAuth::OAuth(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent)
    : QObject(parent)
    , m_appId(appId)
    , m_consumerSecret(consumerSecret)
    , m_redirectUrl(redirectUrl)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_networkReply(0)
{
}


OAuth::~OAuth()
{
}
//
//
// QByteArray OAuth::appId() const
// {
//     return m_appId;
// }
//
//
QByteArray OAuth::authToken() const
{
    return m_authToken;
}


QByteArray OAuth::generateBaseString(QNetworkAccessManager::Operation opType, const QByteArray& url, QList< QPair< QByteArray, QByteArray > > params)
{
    QByteArray baseStr;

    // Convert the HTTP Method to uppercase and set the output string equal to this value
    // and append the '&' character to the output string.
    if (opType == QNetworkAccessManager::PostOperation) {
        baseStr += "POST&";
    } else if (opType == QNetworkAccessManager::GetOperation) {
        baseStr += "GET&";
    }

    // Percent encode the URL and append it to the output string.
    baseStr += QUrl::toPercentEncoding(url);

    // Append the '&' character to the output string.
    baseStr += "&";

    // Percent encode the parameter string and append it to the output string.
    QByteArray paramStr;
    QPair<QByteArray, QByteArray> param;

    // Sort the list of parameters alphabetically
    qSort(params);

    foreach (param, params) {
        paramStr += QUrl::toPercentEncoding(param.first) + "=" + QUrl::toPercentEncoding(param.second) + "&";
    }

    // remove last "&" (not supposed to be there)
    paramStr.chop(1);

    baseStr += QUrl::toPercentEncoding(paramStr);

    return baseStr;
}


QByteArray OAuth::generateRequestAccessTokenHeader(QNetworkAccessManager::Operation opType, QByteArray url)
{
    QByteArray timeStamp = OAuth::timeStamp();
    QByteArray nonce = OAuth::nonce();

    // consumer secret + "&" + "requestToken
    QByteArray key(QUrl::toPercentEncoding(m_consumerSecret));
    key += "&";

    // create list of params for signing
    QList<QPair<QByteArray, QByteArray> > params;

    params.append(QPair<QByteArray, QByteArray>("oauth_nonce", nonce));
    params.append(QPair<QByteArray, QByteArray>("oauth_callback", m_redirectUrl));
    params.append(QPair<QByteArray, QByteArray>("oauth_signature_method", "HMAC-SHA1"));
    params.append(QPair<QByteArray, QByteArray>("oauth_timestamp", timeStamp));
    params.append(QPair<QByteArray, QByteArray>("oauth_consumer_key", m_appId));
    params.append(QPair<QByteArray, QByteArray>("oauth_version", "1.0"));

    QByteArray baseStr = generateBaseString(opType, url, params);
    QByteArray signature = hmacsha1Encode(baseStr, key);

    // and now create the header
    QByteArray header("OAuth ");

    if (!m_redirectUrl.isEmpty()) {
        header += "oauth_callback=\"" + QUrl::toPercentEncoding(m_redirectUrl) + "\",";
    }

    header += "oauth_consumer_key=\"" + m_appId + "\",";
    header += "oauth_nonce=\"" + nonce + "\",";
    header += "oauth_signature=\"" + QUrl::toPercentEncoding(signature) + "\",";
    header += "oauth_signature_method=\"HMAC-SHA1\",";
    header += "oauth_timestamp=\"" + timeStamp + "\",";
    header += "oauth_version=\"1.0\"";

    return header;
}


QByteArray OAuth::hmacsha1Encode(const QByteArray &baseStr, QByteArray key)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard

    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "\"
    // ascii characters 0x36 ("6") and 0x5c ("\") are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }

    // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;

    part.append(baseStr);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));

    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);

    return hashed.toBase64();
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


void OAuth::obtainRequestToken(const QByteArray &requestUrl)
{
    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 1)
    QUrl reqUrlized(requestUrl);
    QNetworkRequest request(reqUrlized);
    request.setRawHeader("Authorization", generateRequestAccessTokenHeader(QNetworkAccessManager::PostOperation, requestUrl));

    m_networkReply = m_networkAccessManager->post(request, QByteArray());

    connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(onObtainRequestTokenReplyRecieved()));
    /// TODO connect error signal
}


void OAuth::onObtainRequestTokenReplyRecieved()
{
    qDebug() << "REQUEST TOKEN RECIEVED: " << m_networkReply->readAll();
}



QByteArray OAuth::redirectUrl() const
{
    return m_redirectUrl;
}


// void OAuth::setAppId(const QByteArray &appId)
// {
//     m_appId = appId;
// }


void OAuth::setAuthToken(const QByteArray &authToken)
{
    m_authToken = authToken;
    Q_EMIT authTokenChanged();
}


// void OAuth::setRedirectUrl(const QByteArray &redirectUrl)
// {
//     m_redirectUrl = redirectUrl;
// }
//
//
//
//

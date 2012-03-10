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
    : OAuth(appId, redirectUrl, parent)
    , m_networkAccessManager(new QNetworkAccessManager(this))
    , m_networkReply(0)
    , m_consumerSecret(consumerSecret)
{
}


Twitter::~Twitter()
{
    m_networkAccessManager->deleteLater();
}


QByteArray Twitter::generateRequestAccessTokenHeader(RequestType type, QByteArray url)
{
    QByteArray timeStamp = OAuth::timeStamp();
    QByteArray nonce = OAuth::nonce();

    // consumer secret + "&" + "requestToken
    QByteArray key(QUrl::toPercentEncoding(m_consumerSecret));
    key += "&";

    // create list of params for signing
    QList<QPair<QByteArray, QByteArray> > params;

    params.append(QPair<QByteArray, QByteArray>("oauth_nonce", nonce));
    params.append(QPair<QByteArray, QByteArray>("oauth_callback", redirectUrl()));
    params.append(QPair<QByteArray, QByteArray>("oauth_signature_method", "HMAC-SHA1"));
    params.append(QPair<QByteArray, QByteArray>("oauth_timestamp", timeStamp));
    params.append(QPair<QByteArray, QByteArray>("oauth_consumer_key", appId()));
    params.append(QPair<QByteArray, QByteArray>("oauth_version", "1.0"));

    QByteArray baseStr = generateBaseString(type, url, params);
    QByteArray signature = hmacsha1Encode(baseStr, key);

    // and now create the header
    QByteArray header("OAuth ");
    header += "oauth_callback=\"" + QUrl::toPercentEncoding(redirectUrl()) + "\",";
    header += "oauth_consumer_key=\"" + appId() + "\",";
    header += "oauth_nonce=\"" + nonce + "\",";
    header += "oauth_signature=\"" + QUrl::toPercentEncoding(signature) + "\",";
    header += "oauth_signature_method=\"HMAC-SHA1\",";
    header += "oauth_timestamp=\"" + timeStamp + "\",";
    header += "oauth_version=\"1.0\"";

    return header;
}


QByteArray Twitter::generateBaseString(RequestType type, const QByteArray &url, QList<QPair<QByteArray, QByteArray> > params)
{
    QByteArray baseStr;

    // Convert the HTTP Method to uppercase and set the output string equal to this value
    // and append the '&' character to the output string.
    if (type == POST) {
        baseStr += "POST&";
    } else if (type == GET) {
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


QString Twitter::obtainAuthPageUrl()
{
    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 2)

    // need access token first
    if (authToken().isEmpty()) {
        /// TODO notify error, we need to request the auth token first
        qDebug("[ERROR] Twitter::obtainAuthPageUrl: need to request token first");
        return QString();
    }

    // once i have the access token, return the authentication url
    QString authPageUrl(AUTHENTICATE_URL);
    authPageUrl += "?auth_token=" + m_requestToken;

    return authPageUrl;
}


void Twitter::onRequestTokenReplyRecieved()
{
    QByteArray received = m_networkReply->readAll();

    // extract token
    QList<QByteArray> chunks = received.split('&');
    QByteArray chunk;

    /// TODO do validity checks here?
    foreach(chunk, chunks) {
        QList<QByteArray>params = chunk.split('=');

        QByteArray key = params.at(0);
        QByteArray value = params.at(1);

#ifdef DEBUG_MODE
        qDebug() << "Twitter::onRequestTokenReplyRecieved RECIEVED: " << received;
        qDebug() << "Key: " << key;
        qDebug() << "Value: " << value;
#endif

        if (key == "oauth_token") {
            m_requestToken = value;
        } else if (key == "oauth_token_secret") {
            m_authTokenSecret = value;
        }
    }

    // Notify that we have the request auth token
    Q_EMIT(requestAccessTokenRecieved());
}


void Twitter::requestAccessToken()
{
    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 1)
    QNetworkRequest request(QUrl(REQUEST_TOKEN_URL));
    request.setRawHeader("Authorization", generateRequestAccessTokenHeader(POST, REQUEST_TOKEN_URL));

    m_networkReply = m_networkAccessManager->post(request, QByteArray());

    connect(m_networkReply, SIGNAL(readyRead()), this, SLOT(onRequestTokenReplyRecieved()));
}





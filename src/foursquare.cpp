/*
 * foursquare.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "qjson/include/QJson/Parser"
#include "foursquare.h"
#include "foursquareuser.h"

#include <QtCore/QDebug>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>

#define AUTH_URL "https://foursquare.com/oauth2/authenticate?"
#define ACCESS_TOKEN_URL "https://foursquare.com/oauth2/access_token?"
#define API_URL "https://api.foursquare.com/v2/"

using namespace Socializer;


Foursquare::Foursquare(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, QByteArray(), parent)
    , m_networkReply(0)
    , m_fqUser(new FoursquareUser(this))
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}


Foursquare::Foursquare(const QByteArray& appId, const QByteArray& redirectUrl, const QByteArray& consumerSecret, QObject* parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
    , m_networkReply(0)
    , m_fqUser(new FoursquareUser(this))
{
    qDebug() << "[Foursquare::Foursquare] secret: " << consumerSecret;
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}


Foursquare::~Foursquare()
{
}


QString Foursquare::dateVerifier() const
{
    qDebug("[Foursquare::dateVerifier]");
    qDebug() << "[Foursquare::dateVerifier] returning: " << "&v=" + QDate::currentDate().toString("yyyyMMdd");
    return "&v=" + QDate::currentDate().toString("yyyyMMdd");
}


void Foursquare::obtainAuthPageUrl()
{
    qDebug("[Foursquare::obtainAuthPageUrl]");

    QString urlStr(AUTH_URL);

    urlStr.append("client_id=");
    urlStr.append(m_appId);

    if (m_consumerSecret.isEmpty()) {
        urlStr.append("&response_type=token&redirect_uri=");
    } else {
        urlStr.append(("&response_type=code&redirect_uri="));
    }

    urlStr.append(m_redirectUrl);

    Q_EMIT authPageUrlReady(urlStr);
}


void Foursquare::onAuthTokenChanged()
{
    qDebug("[Foursquare::onAuthTokenChanged]");

    populateData();
}


void Foursquare::onNetReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[Foursquare::onNetReplyError]");

    // don't need to do check for int conversion here. I trust Qt's code ;)
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString errStr = reply->errorString();

    qDebug() << "[Foursquare::onNetReplyError] Error msg: " << errStr;
    qDebug() << "[Foursquare::onNetReplyError] Status code: " << statusCode;

    reply->deleteLater();

    /// TODO implement cases
    Q_UNUSED(error)
    Q_UNUSED(errStr)
    Q_UNUSED(statusCode)
}


void Foursquare::onPopulateDataReplyReceived()
{
    qDebug("[Foursquare::onPopulateDataReplyReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    qDebug() << "RCV: " << reply->readAll();

    reply->deleteLater();
}


void Foursquare::parseAccessToken()
{
    qDebug("[Foursquare::parseAccessToken]");

    QByteArray incoming = m_networkReply->readAll();

    qDebug() << "[Foursquare::parseAccessToken] incoming: " << incoming;

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(incoming, &ok).toMap();

    if (!ok) {
        qDebug() << "[Foursquare::parseAccessToken] ERROR: " << parser.errorString();
        m_networkReply->deleteLater();
        return;
    }

    setAuthToken(result["access_token"].toByteArray());

    m_networkReply->deleteLater();
}


void Foursquare::parseNewUrl(const QString& url)
{
    qDebug("[Foursquare::parseNewUrl]");
    qDebug() << "[Foursquare::parseNewUrl] got url: " << url;

    if (url.contains("access_token")) {
        QRegExp regex("access_token=?[^&]+");

        if (regex.indexIn(url) > -1) {
            QString access = regex.cap(0);

            // extract access token
            setAuthToken(access.split("=").at(1).toUtf8());

            qDebug() << "[Foursquare::parseNewUrl] Auth token is: " << m_authToken;
        }
    } else if (url.contains("?code=")) {
        QStringList split = url.split("?code=");

        if (split.size() == 2) {
            // request access token via code
            QString accessTokenFromCodeUrl(ACCESS_TOKEN_URL);
            accessTokenFromCodeUrl += "client_id=" + m_appId;
            accessTokenFromCodeUrl += "&client_secret=" + m_consumerSecret;
            accessTokenFromCodeUrl += "&grant_type=authorization_code";
            accessTokenFromCodeUrl += "&redirect_uri=" + m_redirectUrl;
            accessTokenFromCodeUrl += "&code=" + split.at(1);

            qDebug() << "[Foursquare::parseNewUrl] token request url: " << accessTokenFromCodeUrl;

            QNetworkRequest req;
            req.setUrl(accessTokenFromCodeUrl);

            m_networkReply = m_networkAccessManager->get(req);

            connect(m_networkReply, SIGNAL(finished()), this, SLOT(parseAccessToken()));
        }
    }
}


void Foursquare::populateData()
{
    qDebug("[Foursquare::populateData]");

    if (m_authToken.isEmpty()) {
        qDebug("[Foursquare::populateData] no access token has been set!");
        return;
    }

    QNetworkRequest req;
    QNetworkReply *netRep;

    QString reqStr(API_URL);
    reqStr += "users/self?oauth_token=" + m_authToken;
    reqStr += dateVerifier();

    qDebug() << "[Foursquare::populateData] requesting: " << reqStr;

    req.setUrl(QUrl(reqStr));
    netRep = m_networkAccessManager->get(req);

    // connect
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
    connect(netRep, SIGNAL(readyRead()), this, SLOT(onPopulateDataReplyReceived()));
}


void Foursquare::setContextProperty(QDeclarativeView* view)
{
    qDebug("[Foursquare::setContextProperty]");

    view->rootContext()->setContextProperty("Foursquare", this);
}

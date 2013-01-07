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

#include <QtCore/QDebug>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>

#define AUTH_URL "https://foursquare.com/oauth2/authenticate?"
#define ACCESS_TOKEN_URL "https://foursquare.com/oauth2/access_token?"

using namespace Socializer;


Foursquare::Foursquare(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, QByteArray(), parent)
    , m_networkReply(0)
{
}


Foursquare::Foursquare(const QByteArray& appId, const QByteArray& redirectUrl, const QByteArray& consumerSecret, QObject* parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
    , m_networkReply(0)
{
    qDebug() << "[Foursquare::Foursquare] secret: " << consumerSecret;
}



Foursquare::~Foursquare()
{
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


void Foursquare::parseIncomingJson()
{
    qDebug("[Foursquare::parseIncomingJson]");

    QByteArray incoming = m_networkReply->readAll();

    qDebug() << "[Foursquare::parseIncomingJson] incoming: " << incoming;

    QJson::Parser parser;
    bool ok;

    QVariantMap result = parser.parse(incoming, &ok).toMap();

    if (!ok) {
        qDebug() << "[Foursquare::parseIncomingJson] ERROR: " << parser.errorString();
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

            connect(m_networkReply, SIGNAL(finished()), this, SLOT(parseIncomingJson()));
        }
    }
}



void Foursquare::setContextProperty(QDeclarativeView* view)
{
    qDebug("[Foursquare::setContextProperty]");

    view->rootContext()->setContextProperty("Foursquare", this);
}

/*
 * facebook.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "facebook.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#define AUTH_URL "https://m.facebook.com/dialog/oauth?"
#define GRAPH_URL "https://graph.facebook.com/me/"

using namespace Socializer;


Facebook::Facebook(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, QByteArray(), parent)
    , m_scopeEmail(false)
    , m_scopePublishAcions(false)
    , m_scopePublishCheckins(false)
    , m_scopePublishStream(false)
    , m_scopeReadStream(false)
    , m_scopeUserInfo(false)
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}


Facebook::Facebook(const QByteArray &authToken)
    : OAuth(authToken, this)
    , m_scopeEmail(false)
    , m_scopePublishAcions(false)
    , m_scopePublishCheckins(false)
    , m_scopePublishStream(false)
    , m_scopeReadStream(false)
    , m_scopeUserInfo(false)
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}



Facebook::~Facebook()
{
}


QString Facebook::createScope()
{
    QList<QString> scopeList;
    QString scopeLine("&scope=");
    bool isFirst = true;

    if (m_scopeEmail) {
        scopeList.append("email");
    }

    if (m_scopePublishAcions) {
        scopeList.append("publish_actions");
    }

    if (m_scopePublishCheckins) {
        scopeList.append("publish_checkins");
    }

    if (m_scopePublishStream) {
        scopeList.append("publish_stream");
    }

    if (m_scopeReadStream) {
        scopeList.append("read_stream");
    }

    if (m_scopeUserInfo) {
        scopeList.append("user_about_me");
        scopeList.append("user_birthday");
        scopeList.append("user_education_history");
        scopeList.append("user_hometown");
        scopeList.append("user_interests");
        scopeList.append("user_likes");
        scopeList.append("user_religion_politics");
        scopeList.append("user_website");
        scopeList.append("user_work_history");
    }

    foreach(QString scope, scopeList) {
        if (!isFirst) {
            scope.prepend(",");
        } else {
            isFirst = false;
        }

        scopeLine.append(scope);
    }

    if (!isFirst) {
        return scopeLine;
    } else {
        return QString();
    }
}


void Facebook::enableScopeEmail(bool enable)
{
    m_scopeEmail = enable;
}


void Facebook::enableScopePublishActions(bool enable)
{
    m_scopePublishAcions = enable;
}


void Facebook::enableScopePublishCheckins(bool enable)
{
    m_scopePublishCheckins = enable;
}


void Facebook::enableScopePublishStream(bool enable)
{
    m_scopePublishStream = enable;
}


void Facebook::enableScopeReadStream(bool enable)
{
    m_scopeReadStream = enable;
}


void Facebook::enableScopeUserInfo(bool enable)
{
    m_scopeUserInfo = enable;
}


void Facebook::obtainAuthPageUrl()
{
    qDebug("[Facebook::obtainAuthPageUrl]");

    QString urlStr(AUTH_URL);

    urlStr.append("client_id=");
    urlStr.append(m_appId);
    urlStr.append("&redirect_uri=");
    urlStr.append(m_redirectUrl);
    urlStr.append(createScope());
    urlStr.append("&response_type=token");

    Q_EMIT authPageUrlReady(urlStr);
}


void Facebook::onNetReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[Facebook::onNetReplyError]");

    // don't need to do check for int conversion here. I trust Qt's code ;)
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QString errStr = reply->errorString();

    qDebug() << "Error msg: " << errStr;
    qDebug() << "Status code: " << statusCode;


    reply->deleteLater();

    /// TODO implement cases
    Q_UNUSED(error)
    Q_UNUSED(errStr)
    Q_UNUSED(statusCode)

//     qDebug() << "Remaining replies in list: " << m_fbNetReplies.count();
}


void Facebook::onAuthTokenChanged()
{
    qDebug("[Facebook::onAuthTokenChanged]");

    qDebug() << "TOKEN: " << m_authToken;

    populateData();
}


void Facebook::onNetReplyReceived()
{
    qDebug("[Facebook::onNetReplyReceived]");

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = reply->readAll();

    qDebug() << "[Facebook::onNetReplyReceived] rcv: " << rcv;

    reply->deleteLater();
}


void Facebook::parseNewUrl(const QString& url)
{
    qDebug() << "[Facebook::parseNewUrl] got url: " << url;

    if (url.contains("access_token")) {
        QRegExp regex("access_token=?[^&]+");

        if (regex.indexIn(url) > -1) {
            QString access = regex.cap(0);

            // extract access token
            setAuthToken(access.split("=").at(1).toUtf8());

            qDebug() << "[Facebook::parseNewUrl] Auth token is: " << m_authToken;
        }
    }
}


void Facebook::populateData()
{
    qDebug("[Facebook::populateData]");

    if (m_authToken.isEmpty()) {
        qDebug("[Facebook::populateData] no access token has been set!");
        return;
    }

    if (m_scopeUserInfo) {
        QNetworkRequest req;
        QNetworkReply *netRep;

        QString reqUrl(GRAPH_URL);
        reqUrl += "?access_token=" + m_authToken;

        req.setUrl(QUrl(reqUrl));
        netRep = m_networkAccessManager->get(req);

        // connect
        connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
        connect(netRep, SIGNAL(readyRead()), this, SLOT(onNetReplyReceived()));
    }
}


bool Facebook::scopeEmail() const
{
    return m_scopeEmail;
}


void Facebook::setContextProperty(QDeclarativeView *view)
{
    view->rootContext()->setContextProperty("Facebook", this);
}


bool Facebook::scopePublishActions() const
{
    return m_scopePublishAcions;
}


bool Facebook::scopePublishCheckins() const
{
    return m_scopePublishCheckins;
}


bool Facebook::scopePublishStream() const
{
    return m_scopePublishStream;
}


bool Facebook::scopeReadStream() const
{
    return m_scopeReadStream;
}


bool Facebook::scopeUserInfo() const
{
    return m_scopeUserInfo;
}


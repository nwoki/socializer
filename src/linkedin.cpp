/*
 * linkedin.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include "linkedin.h"
#include "linkedinuser.h"

#include <QtCore/QDebug>

#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>

#include <QtXml/QXmlStreamReader>

#include <qjson/parser.h>

#define AUTH_URL "https://www.linkedin.com/uas/oauth2/authorization?"
#define ACCESS_TOKEN_URL "https://www.linkedin.com/uas/oauth2/accessToken?"
#define UPDATE_INFO_URL "https://api.linkedin.com/v1/people/~"

using namespace Socializer;


LinkedIn::LinkedIn(const QByteArray& authToken, QObject* parent)
    : OAuth(authToken, parent)
    , m_linkedinUser(new LinkedInUser(this))
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAccessTokenChanged()));

    populateData();
}


LinkedIn::LinkedIn(const QByteArray &appId, const QByteArray &consumerSecret, const QByteArray &redirectUrl, QObject* parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
    , m_linkedinUser(new LinkedInUser(this))
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAccessTokenChanged()));
}


LinkedIn::~LinkedIn()
{
}


bool LinkedIn::basicProfileScope() const
{
    qDebug("[LinkedIn::basicProfileScope]");

    return m_basicProfileScope;
}


bool LinkedIn::contactInfoScope() const
{
    qDebug("[LinkedIn::contactInfoScope]");

    return m_contactInfoScope;
}


bool LinkedIn::emailProfileScope() const
{
    qDebug("[LinkedIn::emailProfileScope]");

    return m_emailAddressScope;
}


bool LinkedIn::fullProfileScope() const
{
    qDebug("[LinkedIn::fullProfileScope]");

    return m_fullProfileScope;
}


bool LinkedIn::networkScope() const
{
    qDebug("[LinkedIn::networkScope]");

    return m_networkScope;
}


void LinkedIn::setBasicProfileScope(bool enable)
{
    qDebug("[LinkedIn::setBasicProfileScope]");

    m_basicProfileScope = enable;
}


void LinkedIn::setContactInfoScope(bool enable)
{
    qDebug("[LinkedIn::setContactInfoScope]");

    m_contactInfoScope = enable;
}


void LinkedIn::setEmailProfileScope(bool enable)
{
    qDebug("[LinkedIn::setEmailProfileScope]");

    m_emailAddressScope = enable;
}


void LinkedIn::setFullProfileScope(bool enable)
{
    qDebug("[LinkedIn::setFullProfileScope]");

    m_fullProfileScope = enable;
}


void LinkedIn::setNetworkScope(bool enable)
{
    qDebug("[LinkedIn::setNetworkScope]");

    m_networkScope = enable;
}


QString LinkedIn::createScope()
{
    qDebug("[LinkedIn::createScope]");

    QList<QString> scopeList;
    QString scopeLine("&scope=");
    bool isFirst = true;

    if (m_basicProfileScope && !m_fullProfileScope) {
        scopeList.append("r_basicprofile");
    } else if (!m_basicProfileScope && m_fullProfileScope) {
        scopeList.append("r_fullprofile");
    }

    if (m_contactInfoScope) {
        scopeList.append("r_contactinfo");
    }

    if (m_emailAddressScope) {
        scopeList.append("r_emailaddress");
    }

    if (m_networkScope) {
        scopeList.append("r_network");
    }


    Q_FOREACH (const QString &scope, scopeList) {
        if (!isFirst) {
            scopeLine.append(',');
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


bool LinkedIn::isResponseValid(const QByteArray &msg)
{
    qDebug("[LinkedIn::isResponseValid]");

    QXmlStreamReader xmlReader(msg);

    while (!xmlReader.atEnd()) {
        if (xmlReader.readNextStartElement()) {

            // TODO save error code and string
            if (xmlReader.name() == "error") {
                // extract error code and message
                while (!xmlReader.atEnd()) {
                    if (xmlReader.readNextStartElement()) {
                        if (xmlReader.name() == "status") {
                            qDebug() << "[LinkedIn::isResponseValid] ERROR CODE: " << xmlReader.readElementText();
                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}


void LinkedIn::obtainAuthPageUrl()
{
    qDebug("[LinkedIn::obtainAuthPageUrl]");

    QString urlStr(AUTH_URL);

    urlStr.append("response_type=code");
    urlStr.append("&client_id=");
    urlStr.append(m_appId);
    urlStr.append(createScope());
    urlStr.append("&state=");
    urlStr.append(nonce());             // here any value will suffice. To prevent CSRF
    urlStr.append("&redirect_uri=");
    urlStr.append(m_redirectUrl);

    Q_EMIT authPageUrlReady(urlStr);
}


void LinkedIn::onAccessTokenChanged()
{
    qDebug("[LinkedIn::onAccessTokenChanged]");
    populateData();
}


void LinkedIn::onAccessTokenReceived()
{
    qDebug("[LinkedIn::onAccessTokenReceived]");

    QNetworkReply *netReply = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = netReply->readAll();

    qDebug() << "[LinkedIn::onAccessTokenReceived] got: " << rcv;

    netReply->deleteLater();

    // extract auth token
    QJson::Parser parser;
    bool ok;
    QVariantMap jsonMap = parser.parse(rcv, &ok).toMap();

    if (!ok) {
        qDebug("[LinkedIn::onAccessTokenReceived] ERROR in parsing json");
        return;
    }

    setAuthToken(jsonMap.value("access_token").toString().toUtf8());
}


void LinkedIn::parseNewUrl(const QString &url)
{
    // TODO check state matches as well
    // YOUR_REDIRECT_URI/?code=AUTHORIZATION_CODE&state=STATE

    qDebug("[LinkedIn::parseNewUrl]");
    qDebug() << "url: " << url;

    if (url.contains("code")) {
        QRegExp regex("code=?[^&]+");

        if (regex.indexIn(url) > -1) {
            QString access = regex.cap(0);


            // got the code, now i need to exchange it for an access token
            requestAuthToken(access.split('=').at(1));
        }
    }
}


void LinkedIn::onAuthTokenChanged()
{
    qDebug("[LinkedIn::onAuthTokenChanged]");
    qDebug() << "[LinkedIn::onAuthTokenChanged] new token: " << m_authToken;

    populateData();
}


void LinkedIn::onNetReplyError(QNetworkReply::NetworkError error)
{
    qDebug("[LinkedIn::onNetReplyError]");
    // TODO
    Q_UNUSED(error);
}


void LinkedIn::populateData()
{
    qDebug("[LinkedIn::populateData]");

    // for every scope, update use info
    if (m_basicProfileScope || m_fullProfileScope) {
        updateProfileInfo();
    }

    if (m_emailAddressScope) {
        updateEmailInfo();
    }
}


void LinkedIn::requestAuthToken(const QString &code)
{
    qDebug("[LinkedIn::requestAuthToken]");
    qDebug() << "[LinkedIn::requestAuthToken] with code: " << code;

    QNetworkRequest req;
    QNetworkReply *netRep;
    QString reqUrl(ACCESS_TOKEN_URL);

    reqUrl.append("grant_type=authorization_code");
    reqUrl.append("&code=" + code);
    reqUrl.append("&redirect_uri=" + redirectUrl());
    reqUrl.append("&client_id=" + m_appId);
    reqUrl.append("&client_secret=" + m_consumerSecret);

    req.setUrl(reqUrl);

    netRep = m_networkAccessManager->post(req, QByteArray());

    connect(netRep, SIGNAL(finished()), this, SLOT(onAccessTokenReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
}


void LinkedIn::prepareAuthPageUrl()
{
    // once i have the access token, return the authentication url
    QString authPageUrl(AUTH_URL);
    authPageUrl += "?oauth_token=" + m_authToken;

    qDebug() << "[LinkedIn::prepareAuthPageUrl] Emitting auth page url: " << authPageUrl;

    Q_EMIT authPageUrlReady(authPageUrl);
}


void LinkedIn::profileInfoReceived()
{
    qDebug("[LinkedIn::profileInfoReceived]");

    QNetworkReply *rep = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = rep->readAll();

    rep->deleteLater();

    qDebug() << "[LinkedIn::profileInfoReceived] xml received: " << rcv;

    // check response
    if (!isResponseValid(rcv)) {
        return;
    }

    // parse
    QXmlStreamReader xmlParser(rcv);

    while (!xmlParser.atEnd()) {
        if (xmlParser.readNextStartElement()) {
            if (xmlParser.name() == "first-name") {
                m_linkedinUser->setFirstName(xmlParser.readElementText());
            }

            if (xmlParser.name() == "last-name") {
                m_linkedinUser->setLastName(xmlParser.readElementText());
            }

            if (xmlParser.name() == "headline") {
                m_linkedinUser->setHeadLine(xmlParser.readElementText());
            }
        }
    }

//     qDebug() << "\n\nUSE IFNO: " << m_linkedinUser->firstName() << " " << m_linkedinUser->lastName() << " " << m_linkedinUser->headline();
}


void LinkedIn::emailInfoReceived()
{
    qDebug("[LinkedIn::emailInfoReceived]");

    QNetworkReply *rep = qobject_cast<QNetworkReply*>(sender());
    QByteArray rcv = rep->readAll();

    qDebug() << "GOTCHA: " << rcv;

    rep->deleteLater();
}


void LinkedIn::setContextProperty(QDeclarativeView *view)
{
    qDebug("[LinkedIn::setContextProperty]");

    view->rootContext()->setContextProperty("LinkedIn", this);
}


void LinkedIn::updateEmailInfo()
{
    qDebug("[LinkedIn::updateEmailInfo]");

    QString reqUrl(UPDATE_INFO_URL);
    reqUrl.append("/email-address?oauth2_access_token=" + authToken());

    QNetworkRequest req(reqUrl);
    QNetworkReply *netRep = m_networkAccessManager->get(req);

    connect(netRep, SIGNAL(finished()), this, SLOT(emailInfoReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
}


void LinkedIn::updateProfileInfo()
{
    qDebug("[LinkedIn::updateProfileInfo]");

    QString reqUrl(UPDATE_INFO_URL);

    reqUrl.append("?oauth2_access_token=" + authToken());

    QNetworkRequest req(reqUrl);
    QNetworkReply *netRep = m_networkAccessManager->get(req);

    connect(netRep, SIGNAL(finished()), this, SLOT(profileInfoReceived()));
    connect(netRep, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetReplyError(QNetworkReply::NetworkError)));
}





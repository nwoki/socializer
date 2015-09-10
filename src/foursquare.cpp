/*
 * foursquare.cpp
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "foursquare.h"
#include "foursquareuser.h"

#include <QtCore/QDebug>

#ifdef USING_QT5
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#else
#include <qjson/parser.h>
#endif

#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>

#define AUTH_URL "https://foursquare.com/oauth2/authenticate?"
#define ACCESS_TOKEN_URL "https://foursquare.com/oauth2/access_token?"
#define API_URL "https://api.foursquare.com/v2/"

using namespace Socializer;

Foursquare::Foursquare(const QByteArray &authToken, QObject *parent)
    : OAuth(authToken, parent)
    , m_fqUser(new FoursquareUser(this))
{
    // already got access token, populate
    populateData();
}


Foursquare::Foursquare(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent)
    : OAuth(appId, redirectUrl, QByteArray(), parent)
    , m_fqUser(new FoursquareUser(this))
{
    connect(this, SIGNAL(authTokenChanged()), this, SLOT(onAuthTokenChanged()));
}


Foursquare::Foursquare(const QByteArray& appId, const QByteArray& redirectUrl, const QByteArray& consumerSecret, QObject* parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
    , m_fqUser(new FoursquareUser(this))
{
    obtainAuthPageUrl();
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


FoursquareUser* Foursquare::foursquareUser() const
{
    qDebug("[Foursquare::foursquareUser]");

    return m_fqUser;
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

    qDebug() << "[Foursquare::obtainAuthPageUrl] " << urlStr;
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
    QByteArray rcv = reply->readAll();

    reply->deleteLater();

#ifdef USING_QT5
    QJsonObject jsonObj = jsonObject(rcv);
    if (jsonObj.isEmpty()) {
        // error occurred
        return;
    }

    // check response code is valid (meta)
    if (jsonObj.value("meta").toObject().value("code").toVariant().toInt() != 200) {
        // error
        return;
    }

    // main response obj
    QJsonObject responseObj = jsonObj.value("response").toObject();
#else
    QJson::Parser parser;
    bool ok;

    QVariantMap jsonObj = parser.parse(rcv, &ok).toMap();

    if (!ok) {
        return;
    }

    if (jsonObj.value("meta").toMap().value("code").toInt() != 200) {
        return;
    }

    // main response obj
    QVariantMap responseObj = jsonObj.value("response").toMap();
#endif

    // security check (should never be empty)
    if (responseObj.isEmpty()) {
        return;
    }

    // user info
#ifdef USING_QT5
    QJsonObject userObj = responseObj.value("user").toObject();
#else
    QVariantMap userObj = responseObj.value("user").toMap();
#endif

    m_fqUser->setId(userObj.value("id").toString());
    m_fqUser->setFirstName(userObj.value("firstName").toString());
    m_fqUser->setLastName(userObj.value("lastName").toString());
    m_fqUser->setGender(userObj.value("gender").toString());
    m_fqUser->setRelationship(userObj.value("relationship").toString());
    m_fqUser->setHomeCity(userObj.value("homeCity").toString());
    m_fqUser->setBio(userObj.value("bio").toString());

//     qDebug() << m_fqUser->id() << " - " << m_fqUser->lastName() << " - " << m_fqUser->homecity();

    // user photo
#ifdef USING_QT5
    QJsonObject photoObj = userObj.value("photo").toObject();
#else
    QVariantMap photoObj = userObj.value("photo").toMap();
#endif

    m_fqUser->setPhoto(photoObj.value("prefix").toString() + photoObj.value("suffix").toString());

    // contact
#ifdef USING_QT5
    QJsonObject contactObj = userObj.value("contact").toObject();
#else
    QVariantMap contactObj = userObj.value("contact").toMap();
#endif

    m_fqUser->contact().email = contactObj.value("email").toString();
    m_fqUser->contact().facebook = contactObj.value("facebook").toString();
    m_fqUser->contact().phone = contactObj.value("phone").toString();
    m_fqUser->contact().twitter = contactObj.value("twitter").toString();

//     qDebug() << "CONTACTS : " << m_fqUser->contact().email << m_fqUser->contact().facebook << m_fqUser->contact().twitter;

    // last checkin
#ifdef USING_QT5
    QJsonObject checkinsObj = userObj.value("checkins").toObject();
    m_fqUser->setTotalCheckins(checkinsObj.value("count").toVariant().toInt());
#else
    QVariantMap checkinsObj = userObj.value("checkins").toMap();
    m_fqUser->setTotalCheckins(checkinsObj.value("count").toInt());
#endif

//     qDebug() << "Tptl checkins: " << m_fqUser->totalCheckins();

#ifdef USING_QT5
    QJsonArray checkinsArray = checkinsObj.value("items").toArray();
#else
    QList<QVariant> checkinsArray = checkinsObj.value("items").toList();
#endif

    // use last checkin
    if (checkinsArray.size() != 0) {
#ifdef USING_QT5
        QJsonObject lastCheckinObj = checkinsArray.at(0).toObject();
        QJsonObject lastCheckingVenueObj = lastCheckinObj.value("venue").toObject();
#else
        QVariantMap lastCheckinObj = checkinsArray.at(0).toMap();
        QVariantMap lastCheckingVenueObj = lastCheckinObj.value("venue").toMap();
#endif

        m_fqUser->lastCheckin().id = lastCheckingVenueObj.value("id").toString();
        m_fqUser->lastCheckin().name = lastCheckingVenueObj.value("name").toString();
        m_fqUser->lastCheckin().canonicalUrl = lastCheckingVenueObj.value("canonicalUrl").toString();
        m_fqUser->lastCheckin().verified = lastCheckingVenueObj.value("verified").toBool();
        m_fqUser->lastCheckin().isMayor = lastCheckingVenueObj.value("isMayor").toBool();

        // location info
#ifdef USING_QT5
        QJsonObject locationObj = lastCheckingVenueObj.value("location").toObject();

        m_fqUser->lastCheckin().latitude = locationObj.value("lat").toVariant().toDouble();
        m_fqUser->lastCheckin().longitude = locationObj.value("lng").toVariant().toDouble();
#else
        QVariantMap locationObj = lastCheckingVenueObj.value("location").toMap();

        m_fqUser->lastCheckin().latitude = locationObj.value("lat").toDouble();
        m_fqUser->lastCheckin().longitude = locationObj.value("lng").toDouble();
#endif
        m_fqUser->lastCheckin().cc = locationObj.value("cc").toString();
        m_fqUser->lastCheckin().country = locationObj.value("country").toString();

//         qDebug() << "VENUE : " << m_fqUser->lastCheckin().country << " - " << m_fqUser->lastCheckin().name << " - " << m_fqUser->lastCheckin().latitude;
    }

    Q_EMIT profileUpdated();
}


// void Foursquare::parseAccessToken()
// {
//     qDebug("[Foursquare::parseAccessToken]");
//
//     if (!m_networkReply) {
//         return;
//     }
//
//     QByteArray incoming = m_networkReply->readAll();
//
//     qDebug() << "[Foursquare::parseAccessToken] incoming: " << incoming;
//
//     QJson::Parser parser;
//     bool ok;
//
//     QVariantMap result = parser.parse(incoming, &ok).toMap();
//
//     if (!ok) {
//         qDebug() << "[Foursquare::parseAccessToken] ERROR: " << parser.errorString();
//         m_networkReply->deleteLater();
//         return;
//     }
//
//     setAuthToken(result["access_token"].toByteArray());
//
//     m_networkReply->deleteLater();
// }


// void Foursquare::parseNewUrl(const QString& url)
// {
//     qDebug("[Foursquare::parseNewUrl]");
//     qDebug() << "[Foursquare::parseNewUrl] got url: " << url;
//
//     if (url.contains("access_token")) {
//         QRegExp regex("access_token=?[^&]+");
//
//         if (regex.indexIn(url) > -1) {
//             QString access = regex.cap(0);
//
//             // extract access token
//             setAuthToken(access.split("=").at(1).toUtf8());
//
//             qDebug() << "[Foursquare::parseNewUrl] Auth token is: " << m_authToken;
//         }
//     } else if (url.contains("?code=")) {
//         QStringList split = url.split("?code=");
//
//         if (split.size() == 2) {
//             // request access token via code
//             QString accessTokenFromCodeUrl(ACCESS_TOKEN_URL);
//             accessTokenFromCodeUrl += "client_id=" + m_appId;
//             accessTokenFromCodeUrl += "&client_secret=" + m_consumerSecret;
//             accessTokenFromCodeUrl += "&grant_type=authorization_code";
//             accessTokenFromCodeUrl += "&redirect_uri=" + m_redirectUrl;
//             accessTokenFromCodeUrl += "&code=" + split.at(1);
//
//             qDebug() << "[Foursquare::parseNewUrl] token request url: " << accessTokenFromCodeUrl;
//
//             QNetworkRequest req;
//             req.setUrl(accessTokenFromCodeUrl);
//
//             if (!m_networkReply) {
//                 m_networkReply->deleteLater();
//             }
//
//             m_networkReply = m_networkAccessManager->get(req);
//
//             connect(m_networkReply, SIGNAL(finished()), this, SLOT(parseAccessToken()));
//         }
//     }
// }


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
    connect(netRep, SIGNAL(finished()), this, SLOT(onPopulateDataReplyReceived()));
}


void Foursquare::setContextProperty(QDeclarativeView* view)
{
    qDebug("[Foursquare::setContextProperty]");

//    view->rootContext()->setContextProperty("Foursquare", this);
}

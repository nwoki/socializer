/*
 * twitter.cpp
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include "twitter.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#define AUTHENTICATE_URL    "https://api.twitter.com/oauth/authenticate"
#define REQUEST_TOKEN_URL   "https://api.twitter.com/oauth/request_token"
#define ACCESS_TOKEN_URL    "https://api.twitter.com/oauth/access_token"

using namespace Socializer;


Twitter::Twitter(const QByteArray &appId, const QByteArray &redirectUrl, const QByteArray &consumerSecret, QObject *parent)
    : OAuth(appId, redirectUrl, consumerSecret, parent)
{
    connect(this, SIGNAL(requestTokenRecieved()), this, SLOT(prepareAuthPageUrl()));
}


Twitter::~Twitter()
{
}


void Twitter::obtainAuthPageUrl()
{
    // https://dev.twitter.com/docs/auth/implementing-sign-twitter (see step 2)

    // need access token first
    if (m_authToken.isEmpty()) {
        /// TODO notify error, we need to request the auth token first
        qDebug("[ERROR] Twitter::obtainAuthPageUrl: need to obtain request token first");
        obtainRequestToken(REQUEST_TOKEN_URL);
    } else {
        prepareAuthPageUrl();
    }
}


void Twitter::parseNewUrl(const QString &url)
{
#ifdef DEBUG_MODE
    qDebug("[Twitter::parseNewUrl]");
    qDebug() << "url: " << url;
#endif

    QByteArray authVerifier;

    // check that server has returned correct keys after user login
    if (url.contains("oauth_token") && url.contains("oauth_verifier")) {
        // extract part after the "?" from the url
        QStringList sections = url.split('?');

        // extract oauth_token, auth_verifier
        QStringList parts = sections.at(1).split('&');

        // TODO sort twitter auth request validity
//         bool valid = false;

        foreach (QString part, parts) {
            QStringList subPart = part.split('=');

            if (subPart.at(0) == "oauth_token") {
                m_authToken = subPart.at(1).toUtf8();
            } else if (subPart.at(0) == "oauth_verifier") {
                authVerifier = subPart.at(1).toUtf8();
            }
        }

        // check values are not empty
        if (m_authToken.isEmpty() || authVerifier.isEmpty()) {
            /// TODO handle this error (should never be emtpy)
#ifdef DEBUG_MODE
            qDebug("[Twitter::parseNewUrl] ERROR: auth token and auth verifier values are empty!");
#endif
            return;
        }

        // convert the request token to an access token
        // https://dev.twitter.com/docs/auth/implementing-sign-twitter (step 3)
        requestAccessToken(ACCESS_TOKEN_URL, authVerifier);
    }
}


void Twitter::setContextProperty(QDeclarativeView *view)
{
//    view->rootContext()->setContextProperty("Twitter", this);
}


void Twitter::prepareAuthPageUrl()
{
    // once i have the access token, return the authentication url
    QString authPageUrl(AUTHENTICATE_URL);
    authPageUrl += "?oauth_token=" + m_authToken;

#ifdef DEBUG_MODE
    qDebug() << "[Twitter::prepareAuthPageUrl] Emitting auth page url: " << authPageUrl;
#endif

    Q_EMIT authPageUrlReady(authPageUrl);
}


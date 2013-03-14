/*
 * linkedin.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef LINKEDIN_H
#define LINKEDIN_H

#include "oauth.h"

class QDeclarativeView;

namespace Socializer
{

/**
 * LinkedIn interface
 */
class LinkedIn : public OAuth
{
    Q_OBJECT

public:
    /**
     * Creates the Facebook class already with an auth token. Useful for when using Socializer
     * with a web interface delegating the authentication to a web page.
     *
     * @param authToken auth token for the facebook account
     */
    LinkedIn(const QByteArray &authToken, QObject *parent = 0);

    /**
     * Creates the Facebook class ready to obtain the auth token. To use this method you must first set
     * the required permissions and then call "LinkedIn::obtainAuthPageUrl"
     *
     * @param authToken auth token for the facebook account
     */
    LinkedIn(const QByteArray &appId, const QByteArray &consumerSecret, const QByteArray &redirectUrl, QObject *parent = 0);
    ~LinkedIn();

    Q_INVOKABLE void obtainAuthPageUrl();
    Q_INVOKABLE void parseNewUrl(const QString &url);

    /**
     * Use this to set the correct context property in order to use the
     * Socializer::Facebook object with the included FacebookQML component
     * @param view pointer to the Declarative view used in the application
     */
    void setContextProperty(QDeclarativeView *view);

private Q_SLOTS:
    QString createScope();              /** generates the scopes to add to the request url */
    void onAccessTokenReceived();       /** parses json to extract access token */
    void onNetReplyError(QNetworkReply::NetworkError error);

private:
    void onAuthTokenChanged();
    void prepareAuthPageUrl();
    void requestAuthToken(const QString &code);

    bool m_basicProfileScope;           // Name, photo, headline, and current positions
    bool m_fullProfileScope;            // Full profile including experience, education, skills, and recommendations
    bool m_emailAddressScope;           // The primary email address you use for your LinkedIn account
    bool m_networkScope;                // Your 1st and 2nd degree connections
    bool m_contactInfoScope;            // Address, phone number, and bound accounts
};

};

#endif  // LINKEDIN_H
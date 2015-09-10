/*
 * linkedin.h
 *
 * This file is part of the Socializer library
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#ifndef LINKEDIN_H
#define LINKEDIN_H

#include "oauth.h"

class QDeclarativeView;

namespace Socializer
{

class LinkedInUser;

/**
 * LinkedIn interface
 */
class LinkedIn : public OAuth
{
    Q_OBJECT

    Q_PROPERTY(bool basicProfileScope   READ basicProfileScope  WRITE setBasicProfileScope)
    Q_PROPERTY(bool fullProfileScope    READ fullProfileScope   WRITE setFullProfileScope)
    Q_PROPERTY(bool emailProfileScope   READ emailProfileScope  WRITE setEmailProfileScope)
    Q_PROPERTY(bool networkScope        READ networkScope       WRITE setNetworkScope)
    Q_PROPERTY(bool contactInfoScope    READ contactInfoScope   WRITE setContactInfoScope)

public:
    /**
     * Creates the Linkedin class already with an auth token. Useful for when using Socializer
     * with a web interface delegating the authentication to a web page.
     *
     * @param authToken auth token for the facebook account
     */
    LinkedIn(const QByteArray &authToken, bool updateOnCreate = true, QObject *parent = 0);

    /**
     * Creates the Linkedin class with the auth code, and obtains the auth token by itself
     *
     * @param authCode auth code to request the auth token with
     */
    LinkedIn(const QByteArray &authCode, const QByteArray &appId, const QByteArray &consumerSecret, const QByteArray &redirectUrl, QObject *parent = 0);

    ~LinkedIn();

    Socializer::LinkedInUser *userInfo() const;

    bool basicProfileScope() const;
    bool fullProfileScope() const;
    bool emailProfileScope() const;
    bool networkScope() const;
    bool contactInfoScope() const;

    void setBasicProfileScope(bool enable);
    void setFullProfileScope(bool enable);
    void setEmailProfileScope(bool enable);
    void setNetworkScope(bool enable);
    void setContactInfoScope(bool enable);

    // used only for dev to get auth code
    Q_INVOKABLE void obtainAuthPageUrl();

    // DEPRECATED
    Q_INVOKABLE void parseNewUrl(const QString &url) {};

    // DEPRECATED
//     /**
//      * Use this to set the correct context property in order to use the
//      * Socializer::Facebook object with the included FacebookQML component
//      * @param view pointer to the Declarative view used in the application
//      */
//     void setContextProperty(QDeclarativeView *view);

    /**
     * updates the users profile.
     *
     * CAREFUL!
     * if you create this object just to populate data, REMEMBER to set the "lastUpdateTime" of
     * the LinkedinUser if it's not the first time you pull information
     */
    void update();

Q_SIGNALS:
    void profileUpdated();              /** emitted when new profile data has been parsed */

private Q_SLOTS:
    QString createScope();              /** generates the scopes to add to the request url */
    void onAccessTokenChanged();
    void onAccessTokenReceived();       /** parses json to extract access token */
    void onLastUpdatedTimeReceived();
    void onNetReplyError(QNetworkReply::NetworkError error);

    void profileInfoReceived();

private:
    /** calls linkedin api to retreive last update time string */
    void checkLastUpdateTime() const;

    void onAuthTokenChanged();
    void populateData();
    void prepareAuthPageUrl();
    void requestAuthToken(const QString &code);

    bool m_basicProfileScope;           // Name, photo, headline, and current positions
    bool m_fullProfileScope;            // Full profile including experience, education, skills, and recommendations
    bool m_emailAddressScope;           // The primary email address you use for your LinkedIn account
    bool m_networkScope;                // Your 1st and 2nd degree connections
    bool m_contactInfoScope;            // Address, phone number, and bound accounts

    Socializer::LinkedInUser *m_linkedinUser;
};

};

#endif  // LINKEDIN_H


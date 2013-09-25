/*
 * facebook.h
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#ifndef FACEBOOK_H
#define FACEBOOK_H

#include "oauth.h"

#include <QtCore/QDate>

class QDeclarativeView;

namespace Socializer
{

class FacebookUser;

/**
 * Facebook interface
 */

class Facebook : public OAuth
{
    Q_OBJECT

public:
    Facebook(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent = 0);

    /**
     * Creates the Facebook class already with an auth token. Useful for when using Socializer
     * with a web interface delegating the authentication to a web page.
     *
     * @param authToken auth token for the facebook account
     */
    Facebook(const QByteArray &authToken, bool updateOnCreate = true, QObject *parent = 0);

    ~Facebook();

    void enableScopeEmail(bool enable);
    void enableScopePublishActions(bool enable);
    void enableScopePublishCheckins(bool enable);
    void enableScopePublishStream(bool enable);
    void enableScopeReadStream(bool enable);
    void enableScopeUserInfo(bool enable);

    Q_INVOKABLE void obtainAuthPageUrl();
    Q_INVOKABLE void parseNewUrl(const QString &url);

    /**
     * Use this to set the correct context property in order to use the
     * Socializer::Facebook object with the included FacebookQML component
     * @param view pointer to the Declarative view used in the application
     */
    void setContextProperty(QDeclarativeView *view);

    bool scopeEmail() const;
    bool scopePublishActions() const;
    bool scopePublishCheckins() const;
    bool scopePublishStream() const;
    bool scopeReadStream() const;
    bool scopeUserInfo() const;

    FacebookUser *facebookUser() const;             /** returns facebook profile data stored */

    /**
     * updates the users profile.
     *
     * CAREFUL!
     * if you create this object just to populate data, REMEMBER to set the "lastUpdateTime" of
     * the FbUser if it's not the first time you pull information
     */
    void update();

Q_SIGNALS:
    void profileUpdated();      /** emitted when new profile data has been parsed */

private Q_SLOTS:
    void onAuthTokenChanged();
    void onLikeDataReceived();
    void onLastUpdatedTimeReceived();
    void onPopulateDataReplyReceived();
    void onNetReplyError(QNetworkReply::NetworkError error);

private:
    /** calls faceboook api to retreive last update time string */
    void checkLastUpdateTime() const;

    QString createScope();                      /** generates the scopes to add to the request url */
    void parseLikeData(const QByteArray &data);

#ifdef USING_QT5
    void parseLikeData(const QJsonObject &jsonObj);
#else
    void parseLikeData(const QVariantMap &jsonObj);
#endif

    void populateData();                        /** pulls info from all graphs where permissions allow it and populates data */

    // facebook scopes
    bool m_scopeEmail;
    bool m_scopePublishAcions;          /** Enables your app to perform checkins on behalf of the user.*/
    bool m_scopePublishCheckins;        /** Enables your app to post content, comments, and likes to a user's stream and to the streams of the user's friends.*/
    bool m_scopePublishStream;          /** Allows user to publish stories to their ticker, timeline, and news feed using Built-in Actions, Achievements, Scores, or Custom Actions.*/
    bool m_scopeReadStream;             /** Provides access to all the posts in the user's News Feed and enables your application to perform searches against the user's News Feed */
    bool m_scopeUserInfo;               /** Provides info about the user (about, birthday... )*/

    FacebookUser *m_fbUser;                     /** User info */
};

};

#endif  // FACEBOOK_H


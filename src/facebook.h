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

class QDeclarativeView;

namespace Socializer
{

/**
 * Facebook interface
 */

class Facebook : public OAuth
{
    Q_OBJECT

    Q_PROPERTY (bool scopeEmail             READ scopeEmail             WRITE enableScopeEmail)
    Q_PROPERTY (bool scopePublishActions    READ scopePublishActions    WRITE enableScopePublishActions)
    Q_PROPERTY (bool scopePublishCheckins   READ scopePublishCheckins   WRITE enableScopePublishCheckins)
    Q_PROPERTY (bool scopePublishStream     READ scopePublishStream     WRITE enableScopePublishStream)
    Q_PROPERTY (bool scopeReadStream        READ scopeReadStream        WRITE enableScopeReadStream)
    Q_PROPERTY (bool scopeUserInfo          READ scopeUserInfo          WRITE enableScopeUserInfo)

public:

    // friends
    struct Friend {
        QString id;
        QString name;
        QString firstName;
        QString lastName;
        QString picture;
    };

    // game
//     struct Game {
// 
//     };
// 
//     // movie
//     struct Movie {
// 
//     };
// 
//     // music
//     struct Music {
// 
//     };

    // user
    struct Me {
        QString id;
        QString name;
        QString firstName;
        QString lastName;
        QString email;
        QString link;
        QString username;
        QString brithday;       /// TODO make it a QDate?
        QString gender;
        QString relationshipStatus;
        QString picture;
        bool verified;
    };

    Facebook(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent = 0);

    /**
     * Creates the Facebook class already with an auth token. Useful for when using Socializer
     * with a web interface delegating the authentication to a web page.
     *
     * @param authToken auth token for the facebook account
     */
    Facebook(const QByteArray &authToken);

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

private Q_SLOTS:
    void onAuthTokenChanged();
    void onNetReplyReceived();
    void onNetReplyError(QNetworkReply::NetworkError error);

private:
    QString createScope();              /** generates the scopes to add to the request url */
    void populateData();                /** pulls info from all graphs where permissions allow it and populates data */

    // facebook scopes
    bool m_scopeEmail;
    bool m_scopePublishAcions;          /** Enables your app to perform checkins on behalf of the user.*/
    bool m_scopePublishCheckins;        /** Enables your app to post content, comments, and likes to a user's stream and to the streams of the user's friends.*/
    bool m_scopePublishStream;          /** Allows user to publish stories to their ticker, timeline, and news feed using Built-in Actions, Achievements, Scores, or Custom Actions.*/
    bool m_scopeReadStream;             /** Provides access to all the posts in the user's News Feed and enables your application to perform searches against the user's News Feed */
    bool m_scopeUserInfo;               /** Provides info about the user (about, birthday... )*/

    Me m_userInfo;                      /** User info */
    QHash<QString, Friend*>m_friends;   /** List of users friends */ // TODO make QHash<QString, Friend*>. stirng is ID

//     QList<QNetworkReply*>m_fbNetReplies;        /** Network reply class for the facebook class */
};

};

#endif  // FACEBOOK_H
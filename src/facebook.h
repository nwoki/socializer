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

    // common struct for all pairs (id/name)
    struct Pair {
        QString id;
        QString name;
    };

    // education
    struct Education {
        Pair school;
        QString type;
    };

    // friends
    struct Friend {
        QString id;
        QString name;
        QString firstName;
        QString lastName;
        QString gender;
        QString picture;
        QString locale;
        QString username;
        QString link;
        QString relationshipStatus;
        QString about;
        QString birthday;
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

    struct Work {
        Pair employer;
        Pair location;
        Pair position;
        QString description;
        QDate startDate;
        QDate endDate;
    };

    Facebook(const QByteArray &appId, const QByteArray &redirectUrl, QObject *parent = 0);

    /**
     * Creates the Facebook class already with an auth token. Useful for when using Socializer
     * with a web interface delegating the authentication to a web page.
     *
     * @param authToken auth token for the facebook account
     */
    Facebook(const QByteArray &authToken, QObject *parent = 0);

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

    // TODO all this will go in facebookuser
    QList<Friend*> friends() const;             /** returns a list of the user's friends */
    QList<Work*> work() const;                  /** returns the user's list of work experiences */
    QList<Education*> education() const;        /** returns the user's list of education history */

Q_SIGNALS:
    void profileUpdated();      /** emitted when new profile data has been parsed */

private Q_SLOTS:
    void onAuthTokenChanged();
    void onPopulateDataReplyReceived();
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

    FacebookUser *m_fbUser;                     /** User info */

    // TODO all this will go in facebookuser
    QHash<QString, Friend*>m_friends;   /** List of users friends */            // key is ID
    QList<Work*>m_work;                 /** List of the users job history */
    QList<Education*>m_education;       /** List of the users education history */
};

};

#endif  // FACEBOOK_H